#ifndef PACKETCALLONJOIN_H
#define PACKETCALLONJOIN_H

# include	"APacket.hh"
# include	"SerializerBuffer.hpp"
# include       <cstdint>
# include       <string>

class PacketCallOnJoin : public APacket
{
public:
  PacketCallOnJoin();
  ~PacketCallOnJoin();

private:
  PacketCallOnJoin(const PacketCallOnJoin &);
  PacketCallOnJoin &operator=(const PacketCallOnJoin &);

public:
  void			unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer*	serialize() const;

  uint32_t		getUserId() const;
  void			setUserId(uint32_t userId);

  const std::string		&getClientIp() const;
  void			setClientIp(const std::string &clientIp);

  uint16_t		getClientPort() const;
  void			setClientPort(uint16_t clientPort);

private:
  uint32_t	_userId;
  std::string	_clientIp;
  uint16_t	_clientPort;
};

#endif // PACKETCALLONJOIN_H
