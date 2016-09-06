#include	"PacketCreateChannel.hh"

PacketCreateChannel::PacketCreateChannel()
	: APacket(APacket::ID_CREATE_CHANNEL, APacket::CLIENT_TO_SERVER)
{

}

PacketCreateChannel::~PacketCreateChannel()
{

}

void		PacketCreateChannel::unserialize(SerializerBuffer *inBuffer)
{
  try {
    uint16_t size = inBuffer->readToData<uint16_t>();
    while (size)
      {
        _userID.push_back(inBuffer->readToData<uint32_t>());
        size--;
      }
  } catch (const SerializerBufferException &e) {
    std::cerr   << "[PacketCreateChannel] Error : SerializerBufferException failed : "
                << e.what() << std::endl;
  }
}

SerializerBuffer	*PacketCreateChannel::serialize() const
{
  SerializerBuffer* buffer = new SerializerBuffer;

  buffer->writeToData<uint16_t>(_userID.size());
  for (std::list<uint32_t>::const_iterator it = _userID.begin();
       it != _userID.end(); ++it)
    buffer->writeToData<uint32_t>((*it));
  return buffer;
}

const std::list<uint32_t>	&PacketCreateChannel::getUserIds() const
{
  return _userID;
}

void                           PacketCreateChannel::addIdToUserId(uint32_t id)
{
  _userID.push_back(id);
}

