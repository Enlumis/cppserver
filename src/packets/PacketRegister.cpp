#include	"PacketRegister.hh"
#include	"PacketAnswer.hh"
#include	"PacketException.hh"

PacketRegister::PacketRegister()
  : APacket(APacket::ID_REGISTER, APacket::CLIENT_TO_SERVER)
{

}

PacketRegister::~PacketRegister()
{

}

void			PacketRegister::unserialize(SerializerBuffer *inBuffer)
{
  uint16_t	bufferLength;

  try
    {
      bufferLength = inBuffer->readToData<uint16_t>();
      if (bufferLength > m_confManager->get<size_t>("accountNameMaxSize") ||
	  bufferLength < m_confManager->get<size_t>("accountNameMinSize"))
	  throw(PacketException(PacketAnswer::REGISTER_ACCOUNT_NAME_INVALID,
			      "Register packet failed : account name size to big."));
      m_accountName = std::string(inBuffer->readToData(bufferLength));

      bufferLength = inBuffer->readToData<uint16_t>();
      if (bufferLength > m_confManager->get<size_t>("passwordMaxSize") ||
	  bufferLength < m_confManager->get<size_t>("passwordMinSize"))
	throw(PacketException(PacketAnswer::REGISTER_PASSWORD_INVALID,
			      "Register packet failed : hashed passwd size to big "));
      m_passwd = std::string(inBuffer->readToData(bufferLength));
    }
  catch (const SerializerBufferException &e)
    {
      std::cout << "[PacketUpdateUsername] Error : SerializerBufferException failed :"
		<< e.what() << std::endl;
    }
}

SerializerBuffer	*PacketRegister::serialize() const
{
  if (m_accountName.size() > m_confManager->get<size_t>("accountNameMaxSize") ||
      m_accountName.size() < m_confManager->get<size_t>("accountNameMinSize"))
    throw(PacketException(PacketAnswer::REGISTER_ACCOUNT_NAME_INVALID,
			  "Register packet failed : account name size to big."));

  if (m_passwd.size() > m_confManager->get<size_t>("passwordMaxSize") ||
      m_passwd.size() < m_confManager->get<size_t>("passwordMinSize"))
    throw(PacketException(PacketAnswer::REGISTER_PASSWORD_INVALID,
			  "Register packet failed : hashed passwd size to big "));

  SerializerBuffer *buffer = new SerializerBuffer;

  buffer->writeToData<uint16_t>(m_accountName.size());
  buffer->writeToData(m_accountName.c_str(), m_accountName.size());
  buffer->writeToData<uint16_t>(m_passwd.size());
  buffer->writeToData(m_passwd.c_str(), m_passwd.size());
  return buffer;
}

const std::string	&PacketRegister::getAccountName() const
{
  return m_accountName;
}

const std::string	&PacketRegister::getPasswd() const
{
  return m_passwd;
}

void                   PacketRegister::setAccountName(std::string const& accountName)
{
  m_accountName = accountName;
}

void                   PacketRegister::setPasswd(std::string const& passwd)
{
  m_passwd = passwd;
}
