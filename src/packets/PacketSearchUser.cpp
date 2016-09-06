#include "PacketSearchUser.hh"
#include "PacketException.hh"

PacketSearchUser::PacketSearchUser()
  : APacket(ID_SEARCH_USER, CLIENT_TO_SERVER)
{
}

PacketSearchUser::~PacketSearchUser()
{

}

void	PacketSearchUser::unserialize(SerializerBuffer *inBuffer)
{
  try
    {
      uint16_t len = inBuffer->readToData<uint16_t>();
        if (len > m_confManager->get<size_t>("patternMaxSize")
            || len < m_confManager->get<size_t>("patternMinSize"))
        {
			throw(PacketException(PacketAnswer::SEARCH_USER_INVALID_PATTERN,
                "[PacketSearchUser] failed : invalid pattern length : " + len));
        }
      m_pattern = inBuffer->readToData(len);
    } catch (const SerializerBufferException &e) {
    std::cerr << "[PacketSearchUser] Error : SerializerBufferException failed :"
              << e.what() << std::endl;
  }
}

SerializerBuffer* PacketSearchUser::serialize() const
{
  SerializerBuffer* buffer = new SerializerBuffer;

  buffer->writeToData<uint16_t>(m_pattern.size());
  buffer->writeToData(m_pattern.c_str(), m_pattern.size());
  return buffer;
}

std::string PacketSearchUser::getPattern() const
{
  return m_pattern;
}

void PacketSearchUser::setPattern(const std::string &pattern)
{
  m_pattern = pattern;
}
