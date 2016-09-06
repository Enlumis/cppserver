#ifndef PACKETOPENCHANNEL_H
#define PACKETOPENCHANNEL_H

# include	<cstdint>
# include       <list>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"
# include   "Type.hh"
# include   <vector>

class PacketOpenChannel : public APacket
{
public:
  PacketOpenChannel();
  ~PacketOpenChannel();

private:
    std::vector<Type::UserField> _users;

private:
    PacketOpenChannel(const PacketOpenChannel &);
    PacketOpenChannel &operator=(const PacketOpenChannel &);

public:
    SerializerBuffer *serialize() const;
    void            unserialize(SerializerBuffer *inBuffer);

    uint32_t getChannelId() const;
    void setChannelId(const uint32_t &channelId);

    void    setUserdata(uint32_t, std::string const &, std::string const &);
    std::vector<Type::UserField> const &getChannelUsers() const;


private:
    uint32_t              _channelId;
};

#endif // PACKETOPENCHANNEL_H
