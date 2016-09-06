#include	"PacketCallOnQuit.hh"

PacketCallOnQuit::PacketCallOnQuit()
  : APacket(APacket::ID_CALL_ON_QUIT, APacket::SERVER_TO_CLIENT)
{
}

PacketCallOnQuit::~PacketCallOnQuit()
{
}

void PacketCallOnQuit::unserialize(SerializerBuffer *inBuffer)
{
  try{
    _userId = inBuffer->readToData<uint32_t>();
  } catch (const SerializerBufferException &e) {
    std::cerr << "[PacketCallOnQuit] Error : SerializerBufferException failed :"
              << e.what() << std::endl;
  }
}

SerializerBuffer *PacketCallOnQuit::serialize() const
{
  SerializerBuffer	*buffer = new SerializerBuffer;

  buffer->writeToData<uint32_t>(_userId);
  return (buffer);
}

uint32_t		PacketCallOnQuit::getUserId() const
{
  return _userId;
}

void			PacketCallOnQuit::setUserId(uint32_t userId)
{
  _userId = userId;
}
