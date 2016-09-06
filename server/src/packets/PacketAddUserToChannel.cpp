#include "PacketAddUserToChannel.hh"

PacketAddUserToChannel::PacketAddUserToChannel()
  : APacket(ID_ADD_USER_TO_CHANNEL, APacket::CLIENT_TO_SERVER)
{
}

PacketAddUserToChannel::~PacketAddUserToChannel()
{}

void                PacketAddUserToChannel::unserialize(SerializerBuffer *inBuffer)
{
  try {
    _channelId = inBuffer->readToData<uint32_t>();
    _newUserId = inBuffer->readToData<uint32_t>();
  } catch (const SerializerBufferException &e) {
    std::cerr   << "[PacketOpenChannel] Error : SerializerBufferException failed : "
                << e.what() << std::endl;
  }
}

SerializerBuffer*   PacketAddUserToChannel::serialize() const
{
  SerializerBuffer *buffer = new SerializerBuffer();

  buffer->writeToData<uint32_t>(_channelId);
  buffer->writeToData<uint32_t>(_newUserId);
  return buffer;
}

uint32_t PacketAddUserToChannel::getChannelId() const
{
  return _channelId;
}

void PacketAddUserToChannel::setChannelId(uint32_t channelId)
{
  _channelId = channelId;
}
uint32_t PacketAddUserToChannel::getNewUserId() const
{
  return _newUserId;
}

void PacketAddUserToChannel::setNewUserId(uint32_t newUserId)
{
  _newUserId = newUserId;
}
