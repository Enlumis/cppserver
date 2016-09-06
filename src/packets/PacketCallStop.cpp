#include	"PacketCallStop.hh"

PacketCallStop::PacketCallStop()
  : APacket(APacket::ID_CALL_ON_QUIT, APacket::SERVER_TO_CLIENT)
{
}

PacketCallStop::~PacketCallStop()
{
}

void PacketCallStop::unserialize(SerializerBuffer *inBuffer)
{
  try{
    _userId = inBuffer->readToData<uint32_t>();
  } catch (const SerializerBufferException &e) {
    std::cerr << "[PacketCallStop] Error : SerializerBufferException failed :"
              << e.what() << std::endl;
  }
}

SerializerBuffer *PacketCallStop::serialize() const
{
  SerializerBuffer	*buffer = new SerializerBuffer;

  buffer->writeToData<uint32_t>(_userId);
  return (buffer);
}

uint32_t		PacketCallStop::getUserId() const
{
  return _userId;
}

void			PacketCallStop::setUserId(uint32_t userId)
{
  _userId = userId;
}
