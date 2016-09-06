#include	"PacketCallQuit.hh"

PacketCallQuit::PacketCallQuit()
  : APacket(APacket::ID_CALL_QUIT, APacket::CLIENT_TO_SERVER)
{
}

PacketCallQuit::~PacketCallQuit()
{
}

void PacketCallQuit::unserialize(SerializerBuffer *inBuffer)
{
  try{
    _userId = inBuffer->readToData<uint32_t>();
  } catch (const SerializerBufferException &e) {
    std::cerr << "[PacketCallQuit] Error : SerializerBufferException failed :"
              << e.what() << std::endl;
  }
}

SerializerBuffer *PacketCallQuit::serialize() const
{
  SerializerBuffer	*buffer = new SerializerBuffer;

  buffer->writeToData<uint32_t>(_userId);
  return (buffer);
}

uint32_t		PacketCallQuit::getUserId() const
{
  return _userId;
}

void			PacketCallQuit::setUserId(uint32_t userId)
{
  _userId = userId;
}
