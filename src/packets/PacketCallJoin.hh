#ifndef PACKETCALLJOIN_H_
# define PACKETCALLJOIN_H_

#include <cstdint>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"

class PacketCallJoin : public APacket
{
private:
  uint32_t	_userId;
  uint16_t	_port;
public:
  PacketCallJoin();
  ~PacketCallJoin();

private:
  PacketCallJoin(const PacketCallJoin &);
  PacketCallJoin &operator=(const PacketCallJoin &);

public:
  void unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer *serialize() const;
  
public:
  uint32_t	getUserId() const;
  void		setUserId(uint32_t userId);

public:
  uint16_t	getPort() const;
  void		setPort(uint16_t port);
};

#endif /* !PACKETCALLONJOIN_HH_ */
