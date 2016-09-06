#ifndef PACKETADDUSERTOCHANNEL_H
#define PACKETADDUSERTOCHANNEL_H

# include	<cstdint>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"

class PacketAddUserToChannel : public APacket
{
public:
  PacketAddUserToChannel();
  ~PacketAddUserToChannel();

private:
  PacketAddUserToChannel(const PacketAddUserToChannel &);
  PacketAddUserToChannel &operator=(const PacketAddUserToChannel &);

public:
  void            unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer *serialize() const;

  uint32_t	getChannelId() const;
  void		setChannelId(uint32_t channelId);

  uint32_t	getNewUserId() const;
  void		setNewUserId(uint32_t userId);

private:
  uint32_t        _channelId;
  uint32_t        _newUserId;
};

#endif // PACKETADDUSERTOCHANNEL_H
