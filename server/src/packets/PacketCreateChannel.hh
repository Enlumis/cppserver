#ifndef PACKETCREATECHANNEL_H_
# define PACKETCREATECHANNEL_H_

# include	<string>
# include	<cstdint>
# include       <list>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"

class PacketCreateChannel : public APacket
{

public:
  PacketCreateChannel();
  ~PacketCreateChannel();

private:
  PacketCreateChannel(const PacketCreateChannel &);
  PacketCreateChannel &operator=(const PacketCreateChannel &);

public:
  void unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer *serialize() const;

  const std::list<uint32_t>	&getUserIds() const;
  void                           addIdToUserId(uint32_t);

private:
  std::list<uint32_t>		_userID;
};

#endif /* !PACKETCREATECHANNEL_H_ */
