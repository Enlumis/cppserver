#ifndef PACKETCALLONQUIT_H_
# define PACKETCALLONQUIT_H_

#include <cstdint>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"

class PacketCallOnQuit : public APacket
{
private:
  uint32_t	_userId;

public:
  PacketCallOnQuit();
  ~PacketCallOnQuit();

private:
  PacketCallOnQuit(const PacketCallOnQuit &);
  PacketCallOnQuit &operator=(const PacketCallOnQuit &);

public:
  void unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer *serialize() const;

public:
  uint32_t	getUserId() const;
  void		setUserId(uint32_t userId);
};

#endif /* !PACKETCALLONQUIT_HH_ */
