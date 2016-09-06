#include	"PacketUpdateUsername.hh"
#include	"PacketAnswer.hh"
#include	"PacketException.hh"

PacketUpdateUsername::PacketUpdateUsername()
  : APacket(APacket::ID_UPDATE_USERNAME, APacket::CLIENT_TO_SERVER)
{

}

PacketUpdateUsername::~PacketUpdateUsername()
{

}

void PacketUpdateUsername::unserialize(SerializerBuffer *inBuffer)
{
  uint16_t	bufferLength;

  try
    {
      bufferLength = inBuffer->readToData<uint16_t>();
      if (bufferLength > m_confManager->get<size_t>("usernameMaxSize") ||
	  bufferLength < m_confManager->get<size_t>("usernameMinSize"))
	throw(PacketException(PacketAnswer::UPDATE_USERNAME_INVALID,
			      "PacketUpdateUsername failed : invalid username length : " + bufferLength));
      m_username = std::string(inBuffer->readToData(bufferLength));
    }
  catch (const SerializerBufferException &e)
    {
      std::cout << "[PacketUpdateUsername] Error : SerializerBufferException failed :"
		<< e.what() << std::endl;
    }
}

SerializerBuffer *PacketUpdateUsername::serialize() const
{
  if (m_username.size() > m_confManager->get<size_t>("usernameMaxSize") ||
      m_username.size() < m_confManager->get<size_t>("usernameMinSize"))
    throw(PacketException(PacketAnswer::UPDATE_USERNAME_INVALID,
			  "PacketUpdateUsername failed : invalid username length : " + m_username.size()));

  SerializerBuffer *buffer = new SerializerBuffer;

  buffer->writeToData<uint16_t>(m_username.size());
  buffer->writeToData(m_username.c_str(), m_username.size());
  return buffer;
}

const std::string	&PacketUpdateUsername::getUsername() const
{
  return m_username;
}

void                   PacketUpdateUsername::setUsername(std::string const& username)
{
  m_username = username;
}
