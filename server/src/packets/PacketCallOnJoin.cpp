#include "PacketCallOnJoin.hh"

PacketCallOnJoin::PacketCallOnJoin()
  : APacket(APacket::ID_CALL_ON_JOIN, APacket::SERVER_TO_CLIENT)
{
}

PacketCallOnJoin::~PacketCallOnJoin()
{}

void			PacketCallOnJoin::unserialize(SerializerBuffer *inBuffer)
{
  try{
    _userId = inBuffer->readToData<uint32_t>();
    uint16_t clientIpSize = inBuffer->readToData<uint16_t>();
    _clientIp = inBuffer->readToData(clientIpSize);
    _clientPort = inBuffer->readToData<uint16_t>();
  } catch (const SerializerBufferException &e) {
    std::cerr << "[PacketCallOnJoin] Error : SerializerBufferException failed :"
              << e.what() << std::endl;
  }

}

SerializerBuffer*	PacketCallOnJoin::serialize() const
{
  SerializerBuffer *buffer = new SerializerBuffer;

  buffer->writeToData<uint32_t>(_userId);
  buffer->writeToData<uint16_t>(_clientIp.size());
  buffer->writeToData(_clientIp, _clientIp.size());
  buffer->writeToData<uint16_t>(_clientPort);
  return buffer;
}

uint32_t		PacketCallOnJoin::getUserId() const
{
  return _userId;
}

void			PacketCallOnJoin::setUserId(uint32_t userId)
{
  _userId = userId;
}

const std::string		&PacketCallOnJoin::getClientIp() const
{
  return _clientIp;
}

void			PacketCallOnJoin::setClientIp(const std::string &clientIp)
{
  _clientIp = clientIp;
}

uint16_t		PacketCallOnJoin::getClientPort() const
{
  return _clientPort;
}

void			PacketCallOnJoin::setClientPort(uint16_t clientPort)
{
  _clientPort = clientPort;
}
