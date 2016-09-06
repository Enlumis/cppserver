#include	<iostream>
#include	"PacketAuthentification.hh"
#include	"PacketAnswer.hh"
#include	"PacketException.hh"

PacketAuthentification::PacketAuthentification()
  : APacket(APacket::ID_AUTHENTIFICATION, APacket::CLIENT_TO_SERVER)
{

}

PacketAuthentification::~PacketAuthentification()
{

}

void			PacketAuthentification::unserialize(SerializerBuffer *inBuffer)
{
  uint16_t	bufferLength;

  try
    {
      bufferLength = inBuffer->readToData<uint16_t>();
      if (bufferLength > m_confManager->get<size_t>("accountNameMaxSize")
        || bufferLength < m_confManager->get<size_t>("accountNameMinSize"))
      {
        throw(PacketException(PacketAnswer::AUTH_BAD_ACCOUNT_NAME,
			      "Auth packet failed : account name size too big : " + bufferLength));
      }
      m_accountName = std::string(inBuffer->readToData(bufferLength));

      bufferLength = inBuffer->readToData<uint16_t>();
      if (bufferLength > m_confManager->get<size_t>("passwordMaxSize")
        || bufferLength < m_confManager->get<size_t>("passwordMinSize"))
      {
      	throw(PacketException(PacketAnswer::AUTH_BAD_PASSWORD,
        "Auth packet failed : hashed passwd size too big : " + bufferLength));
      }
      m_hashedPasswd = std::string(inBuffer->readToData(bufferLength));
    }
  catch (const SerializerBufferException &e)
    {
      std::cout << "[PacketAuthentification] Error : SerializerBufferException failed :"
		<< e.what() << std::endl;
    }
}

// Server -> Client, Used on client side.
SerializerBuffer	*PacketAuthentification::serialize() const
{
  if (m_accountName.length() > m_confManager->get<size_t>("accountNameMaxSize") ||
      m_accountName.length() < m_confManager->get<size_t>("accountNameMinSize"))
    throw(PacketException(PacketAnswer::AUTH_BAD_ACCOUNT_NAME,
			  "Auth packet failed : account name size too big : " + m_accountName.length()));

  if (m_hashedPasswd.length() > m_confManager->get<size_t>("passwordMaxSize") ||
      m_hashedPasswd.length() < m_confManager->get<size_t>("passwordMinSize"))
    throw(PacketException(PacketAnswer::AUTH_BAD_PASSWORD,
			  "Auth packet failed : hashed passwd size too big : " + m_hashedPasswd.length()));

  SerializerBuffer  *buffer = new SerializerBuffer();

  buffer->writeToData<uint16_t>(this->m_accountName.length());
  buffer->writeToData(this->m_accountName.c_str(), this->m_accountName.length());
  buffer->writeToData<uint16_t>(this->m_hashedPasswd.length());
  buffer->writeToData(this->m_hashedPasswd.c_str(), this->m_hashedPasswd.length());
  return (buffer);
}

// Server -> Client, Used on client side.
void              PacketAuthentification:: setAccountName(std::string const &accountName)
{
  this->m_accountName = accountName;
}

// Server -> Client, Used on client side.
void              PacketAuthentification:: setHashedPasswd(std::string const &hashedPasswd)
{
  this->m_hashedPasswd = hashedPasswd;
}

const std::string	&PacketAuthentification::getAccountName() const
{
  return m_accountName;
}

const std::string	&PacketAuthentification::getHashedPasswd() const
{
  return m_hashedPasswd;
}
