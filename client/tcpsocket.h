#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QTimer>
#include <QPixmap>
#include "ISocket.hh"
#include "PacketHandler.hh"

class Babel;

class TcpSocket : public QObject, public ISocket
{
  Q_OBJECT

  QTcpSocket _socket;

  Babel           *_parent;
  bool            _isConnected;
  PacketHandler   _packetHandler;

  struct packetHeader
  {
    uint16_t id;
    uint16_t size;
  };

public:
  TcpSocket(const QString &ip, quint16 port, QObject *parent = 0);
  int   recv();
  int   send(APacket *packet);
  QTcpSocket const& getSocket() const {return _socket;}
  void  close();

signals:

private slots:
  void    onReadyRead();
  void    connectionTimeOut(QAbstractSocket::SocketError error);
  void    checkIsConnected();
};

#endif // TCPSOCKET_H
