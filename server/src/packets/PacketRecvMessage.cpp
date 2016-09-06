#include "PacketRecvMessage.hh"

PacketRecvMessage::PacketRecvMessage()
  : APacket(APacket::ID_RECV_MESSAGE, APacket::SERVER_TO_CLIENT)
{
}

PacketRecvMessage::~PacketRecvMessage()
{}

void			PacketRecvMessage::unserialize(SerializerBuffer *inBuffer)
{
  try{
      _type = inBuffer->readToData<uint8_t>();
      _id = inBuffer->readToData<uint32_t>();
      _senderId = inBuffer->readToData<uint32_t>();
      uint16_t len = inBuffer->readToData<uint16_t>();
      _message = inBuffer->readToData(len);
  } catch (const SerializerBufferException &e) {
    std::cerr << "[PacketRecvMessage] Error : SerializerBufferException failed :"
              << e.what() << std::endl;
  }
}

SerializerBuffer*	PacketRecvMessage::serialize() const
{
  SerializerBuffer *buffer = new SerializerBuffer;

  buffer->writeToData<uint8_t>(_type);
  buffer->writeToData<uint32_t>(_id);
  buffer->writeToData<uint32_t>(_senderId);
  buffer->writeToData<uint16_t>(_message.size());
  buffer->writeToData(_message, _message.size());
  return buffer;
}


uint32_t PacketRecvMessage::getChanOrUserId() const
{
  return _id;
}

void PacketRecvMessage::setChanOrUserId(const uint32_t &id)
{
  _id = id;
}

uint8_t		PacketRecvMessage::getType() const
{
  return _type;
}

void		PacketRecvMessage::setType(uint8_t type)
{
  _type = type;
}

uint32_t PacketRecvMessage::getSenderId() const
{
  return _senderId;
}

void PacketRecvMessage::setSenderId(const uint32_t &senderId)
{
  _senderId = senderId;
}

std::string PacketRecvMessage::getMessage() const
{
  return _message;
}

void PacketRecvMessage::setMessage(const std::string &message)
{
  _message = message;
}
