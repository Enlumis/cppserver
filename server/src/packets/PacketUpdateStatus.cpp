#include	"PacketUpdateStatus.hh"
#include	"PacketAnswer.hh"
#include	"PacketException.hh"

PacketUpdateStatus::PacketUpdateStatus()
  : APacket(APacket::ID_UPDATE_STATUS, APacket::CLIENT_TO_SERVER)
{

}

PacketUpdateStatus::~PacketUpdateStatus()
{

}

void PacketUpdateStatus::unserialize(SerializerBuffer *inBuffer)
{
  uint16_t	bufferLength;

  try
    {
      bufferLength = inBuffer->readToData<uint16_t>();
      if (bufferLength > m_confManager->get<size_t>("statusMaxSize") ||
	  bufferLength < m_confManager->get<size_t>("statusMinSize"))
	throw(PacketException(PacketAnswer::UPDATE_STATUS_INVALID,
			      "PacketUpdateUsernam failed : status size to big : " + bufferLength));
      m_status = std::string(inBuffer->readToData(bufferLength));
    }
  catch (const SerializerBufferException &e)
    {
      std::cout << "[PacketUpdateUsername] Error : SerializerBufferException failed :"
		<< e.what() << std::endl;
    }
}

SerializerBuffer *PacketUpdateStatus::serialize() const
{
  if (m_status.size() > m_confManager->get<size_t>("statusMaxSize") ||
      m_status.size() < m_confManager->get<size_t>("statusMinSize"))
    throw(PacketException(PacketAnswer::UPDATE_STATUS_INVALID,
			  "PacketUpdateUsernam failed : status size to big : " + m_status.size()));

  SerializerBuffer *buffer = new SerializerBuffer;

  buffer->writeToData<uint16_t>(m_status.size());
  buffer->writeToData(m_status.c_str(), m_status.size());
  return buffer;
}

const std::string	&PacketUpdateStatus::getStatus() const
{
  return m_status;
}

void                    PacketUpdateStatus::setStatus(const std::string& status)
{
  m_status = status;
}
