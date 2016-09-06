#include "PacketSendMessage.hh"

PacketSendMessage::PacketSendMessage()
  : APacket(APacket::ID_SEND_MESSAGE, APacket::CLIENT_TO_SERVER)
{
}

PacketSendMessage::~PacketSendMessage()
{}

void			PacketSendMessage::unserialize(SerializerBuffer *inBuffer)
{
  try{
      _type = inBuffer->readToData<uint8_t>();
      _id = inBuffer->readToData<uint32_t>();
      uint16_t len = inBuffer->readToData<uint16_t>();
      _message = inBuffer->readToData(len);
  } catch (const SerializerBufferException &e) {
    std::cerr << "[PacketSendMessage] Error : SerializerBufferException failed :"
              << e.what() << std::endl;
  }

}

SerializerBuffer*	PacketSendMessage::serialize() const
{
  SerializerBuffer *buffer = new SerializerBuffer;

  buffer->writeToData<uint8_t>(_type);
  buffer->writeToData<uint32_t>(_id);
  buffer->writeToData<uint16_t>(_message.size());
  buffer->writeToData(_message, _message.size());
  return buffer;
}

uint8_t		PacketSendMessage::getType() const
{
  return _type;
}

void		PacketSendMessage::setType(uint8_t type)
{
  _type = type;
}

uint32_t PacketSendMessage::getChanOrUserId() const
{
  return _id;
}

void PacketSendMessage::setChanOrUserId(const uint32_t &id)
{
  _id = id;
}

const std::string &PacketSendMessage::getMessage() const
{
  return _message;
}

void PacketSendMessage::setMessage(const std::string &message)
{
  _message = message;
}
