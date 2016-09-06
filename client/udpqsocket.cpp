#include "udpqsocket.h"
#include <QMessageBox>
#include "AUdpPacket.hh"

UdpQSocket::UdpQSocket(uint32_t port) :
  QObject(nullptr), _udpSocket(this), _port(port)
{
  _udpSocket.bind(QHostAddress::Any, port);
}

UdpQSocket::~UdpQSocket()
{

}

int			UdpQSocket::recv()
{
  std::string		str;
  QHostAddress		address;
  struct packetHeader	header;

  quint64		size = _udpSocket.pendingDatagramSize();
  str.resize(size);
  quint16		tmpPort = static_cast<quint16>(_port);

  _udpSocket.readDatagram(reinterpret_cast<char*>(&header), sizeof(header), &address, &tmpPort);
  if (!(header.id == APacket::ID_VOICE || header.id == APacket::ID_ON_VOICE))
    return (-1);

  _udpSocket.readDatagram(&str[0], size, &address, &tmpPort);
  SerializerBuffer	*buffer = new SerializerBuffer(&str[0], size);
  _packetHandler.handle(static_cast<APacket::packetID>(header.id), buffer, address.toString().toStdString());
  return (0);
}

int			 UdpQSocket::send(APacket *packet)
{

  if (!_udpSocket.isOpen())
    return (-1);
  AUdpPacket		*udpPacket = NULL;
  if (!(udpPacket = dynamic_cast<AUdpPacket*>(packet)))
    return (-1);

  QHostAddress		address(QString::fromStdString(udpPacket->getDestIp()));
  quint16		port = udpPacket->getDestPort();
  std::string		data;
  struct packetHeader	header;

  if (packet->getDirection() != APacket::CLIENT_TO_SERVER)
    throw std::exception();
  try {
    SerializerBuffer	*serialized = packet->serialize();
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
  _udpSocket.writeDatagram(val, address, port);
  return (0);
}

void              UdpQSocket::close()
{
  _udpSocket.close();
}
