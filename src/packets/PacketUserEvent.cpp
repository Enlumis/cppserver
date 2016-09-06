#include "PacketUserEvent.hh"

PacketUserEvent::PacketUserEvent()
  : APacket(ID_USER_EVENT, SERVER_TO_CLIENT)
{
}

PacketUserEvent::~PacketUserEvent()
{
}

void                PacketUserEvent::unserialize(SerializerBuffer *inBuffer)
{
  try
    {
      _userId = inBuffer->readToData<uint32_t>();
      _idEvent = inBuffer->readToData<uint8_t>();
      uint16_t len = inBuffer->readToData<uint16_t>();
      _data = inBuffer->readToData(len);
    } catch (const SerializerBufferException &e) {
    std::cerr << "[PacketUpdateUsername] Error : SerializerBufferException failed :"
              << e.what() << std::endl;
  }
}

SerializerBuffer *  PacketUserEvent::serialize() const
{
  SerializerBuffer *buffer = new SerializerBuffer;

  buffer->writeToData<uint32_t>(_userId);
  buffer->writeToData<uint8_t>(_idEvent);
  buffer->writeToData<uint16_t>(_data.size());
  buffer->writeToData(_data.c_str(), _data.size());
  return buffer;
}

uint32_t PacketUserEvent::getUserId() const
{
  return _userId;
}

void PacketUserEvent::setUserId(uint32_t userId)
{
  _userId = userId;
}

uint8_t PacketUserEvent::getIdEvent() const
{
  return _idEvent;
}

void PacketUserEvent::setIdEvent(uint8_t idEvent)
{
  _idEvent = idEvent;
}

std::string PacketUserEvent::getData() const
{
  return _data;
}

void PacketUserEvent::setData(const std::string &data)
{
  _data = data;
}
