#include "tcpsocket.h"
#include "babel.h"
#include "connectdiag.h"
#include <QMatrix>
#include <QTimer>
#include <QFileDialog>
#include <QTransform>

#include <exception>
#include <QMessageBox>
#include "APacket.hh"
#include "PacketHandler.hh"

TcpSocket::TcpSocket(const QString& ip, quint16 port, QObject *parent)
  : QObject(parent)
  , _socket()
  , _parent(static_cast<Babel*>(parent))
  , _isConnected(false)
  , _packetHandler()
{
  _socket.connectToHost(ip, port);
  if (!_socket.waitForConnected(25*1000))
      throw std::exception();
  _isConnected = true;
  connect(&_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
  QTimer::singleShot(300, this, SLOT(checkIsConnected()));
}

void                    TcpSocket::onReadyRead()
{
  recv();
}

int                     TcpSocket::recv()
{
  struct packetHeader	packet;

  _socket.read(reinterpret_cast<char*>(&packet), sizeof(struct packetHeader));
  QByteArray		data = _socket.read(packet.size);
  SerializerBuffer	*buf = new SerializerBuffer(data.constData(), packet.size);

  qDebug() << "[Debug] Recv Packet id = " << packet.id << ".";
  qDebug() << "[Debug] Packet Size = " << packet.size << ".";
  qDebug() << ((_packetHandler.handle(static_cast<APacket::packetID>(packet.id), buf)) ? "Packet handled !" : "Packet not handled !");

  if (!_socket.atEnd())
    emit		_socket.readyRead();
  return 0;
}

int                     TcpSocket::send(APacket *packet)
{
  if (!_socket.isOpen())
    return (-1);
  std::string		data;
  struct packetHeader	header;
  if (packet->getDirection() != APacket::CLIENT_TO_SERVER)
    throw std::exception();
  try {
    SerializerBuffer *serialized = packet->serialize();
    header.size = serialized->size();
    data = serialized->readToData(header.size);
  } catch (std::exception &e) {
    QMessageBox::warning(NULL, QString("error"), QString(e.what()),
                         QMessageBox::Ok, QMessageBox::NoButton);
    return -1;
  }
  header.id = packet->getID();
  QByteArray		val = QByteArray(reinterpret_cast<char*>(&header),
                              sizeof(header)).append(data.c_str(), data.size());
  if(_socket.write(val) == -1)
    {
      QMessageBox::critical(bab, QString("Connection lost"),
                            QString("The connection with the server is lost !"),
                            QMessageBox::Ok);
      throw std::runtime_error("[Connection lost]");
    }
  return 0;
}

void                    TcpSocket::close()
{
  _socket.close();
}

void			TcpSocket::connectionTimeOut(QAbstractSocket::SocketError)
{
  ConnectDiag		*codiag = _parent->getCoDiag();
  codiag->notify(true, "Impossible to connect to the server (Timed out) !");
}

void    TcpSocket::checkIsConnected()
{
  if (_socket.state() != QAbstractSocket::ConnectedState)
    {
      QMessageBox::critical(bab, QString("Connection lost"),
                            QString("The connection with the server is lost !"),
                            QMessageBox::Ok);
      throw std::runtime_error("[Connection lost]");
    }
  QTimer::singleShot(300, this, SLOT(checkIsConnected()));
}
