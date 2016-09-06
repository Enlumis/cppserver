#ifndef PACKETCALLSTOP_H_
# define PACKETCALLSTOP_H_

#include <cstdint>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"

class PacketCallStop : public APacket
{
private:
  uint32_t	_userId;

public:
  PacketCallStop();
  ~PacketCallStop();

private:
  PacketCallStop(const PacketCallStop &);
  PacketCallStop &operator=(const PacketCallStop &);

public:
  void unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer *serialize() const;

public:
  uint32_t	getUserId() const;
  void		setUserId(uint32_t userId);
};

#endif /* !PACKETCALLONSTOP_HH_ */
