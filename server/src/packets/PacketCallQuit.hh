#ifndef PACKETCALLQUIT_H_
# define PACKETCALLQUIT_H_

#include <cstdint>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"

class PacketCallQuit : public APacket
{
private:
  uint32_t	_userId;

public:
  PacketCallQuit();
  ~PacketCallQuit();

private:
  PacketCallQuit(const PacketCallQuit &);
  PacketCallQuit &operator=(const PacketCallQuit &);

public:
  void unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer *serialize() const;

public:
  uint32_t	getUserId() const;
  void		setUserId(uint32_t userId);
};

#endif /* !PACKETCALLQUIT_HH_ */
