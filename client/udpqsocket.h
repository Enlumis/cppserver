#ifndef UDPQSOCKET_H
#define UDPQSOCKET_H

#include <QWidget>
#include <QUdpSocket>
#include <QHostAddress>
#include "ISocket.hh"
#include "APacket.hh"
#include "PacketHandler.hh"

class UdpQSocket : public QObject, public ISocket
{
  Q_OBJECT
  QUdpSocket      _udpSocket;
  quint32         _port;
  PacketHandler   _packetHandler;

public:
  UdpQSocket(uint32_t port);
  ~UdpQSocket();
  int       recv();
  int       send(APacket *packet);
  void      close();
  struct    packetHeader
  {
    uint16_t id;
    uint16_t size;
  };

signals:

public slots:

};

#endif // UDPQSOCKET_H
