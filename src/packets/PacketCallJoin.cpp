#include	"PacketCallJoin.hh"

PacketCallJoin::PacketCallJoin()
  : APacket(APacket::ID_CALL_JOIN, APacket::CLIENT_TO_SERVER)
{
}

PacketCallJoin::~PacketCallJoin()
{
}

void PacketCallJoin::unserialize(SerializerBuffer *inBuffer)
{
  try{
    _userId = inBuffer->readToData<uint32_t>();
    _port = inBuffer->readToData<uint16_t>();
  } catch (const SerializerBufferException &e) {
    std::cerr << "[PacketCallJoin] Error : SerializerBufferException failed :"
              << e.what() << std::endl;
  }
}

SerializerBuffer	*PacketCallJoin::serialize() const
{
  SerializerBuffer	*buffer = new SerializerBuffer;

  buffer->writeToData<uint32_t>(_userId);
  buffer->writeToData<uint16_t>(_port);
  return (buffer);
}

uint32_t		PacketCallJoin::getUserId() const
{
  return _userId;
}

void			PacketCallJoin::setUserId(uint32_t userId)
{
  _userId = userId;
}

uint16_t		PacketCallJoin::getPort() const
{
  return _port;
}

void			PacketCallJoin::setPort(uint16_t port)
{
  _port = port;
}
