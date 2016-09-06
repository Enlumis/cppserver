#include	"PacketContactRequest.hh"
#include	"PacketAnswer.hh"
#include	"PacketException.hh"

PacketContactRequest::  PacketContactRequest()
    : APacket(APacket::ID_CONTACT_REQUEST, APacket::SERVER_TO_CLIENT)
{

}

PacketContactRequest::  ~PacketContactRequest()
{

}

void                PacketContactRequest::unserialize(SerializerBuffer *inBuffer)
{
  uint16_t        size;

  try {
    this->_contact.uid = inBuffer->readToData<uint32_t>();

    size = inBuffer->readToData<uint16_t>();
    if (size > m_confManager->get<size_t>("usernameMaxSize")
      || size < m_confManager->get<size_t>("usernameMinSize"))
    {
      throw(PacketException(PacketAnswer::CONTACT_REQUEST_INVALID_USERNAME,
			    "[PacketContactRequest] failed : invalid username length : " + size));
    }

    this->_contact.username = std::string(inBuffer->readToData(size));

    size = inBuffer->readToData<uint16_t>();
    if (size > m_confManager->get<size_t>("statusMaxSize")
      || size < m_confManager->get<size_t>("statusMinSize"))
    {
      throw(PacketException(PacketAnswer::CONTACT_REQUEST_INVALID_STATUS,
			    "[PacketContactRequest] failed : invalid status length : " + size));
    }

    this->_contact.status = std::string(inBuffer->readToData(size));
  } catch (SerializerBufferException const &e) {
    std::cerr << "[PacketLoginSuccess] Error : " << e.what() << "." << std::endl;
  }
}

SerializerBuffer    *PacketContactRequest::serialize() const
{
  if (this->_contact.username.size() > m_confManager->get<size_t>("usernameMaxSize")
    || this->_contact.username.size() < m_confManager->get<size_t>("usernameMinSize"))
  {
    throw(PacketException(PacketAnswer::CONTACT_REQUEST_INVALID_USERNAME,
			  "[PacketContactRequest] failed : invalid username length : " + this->_contact.username.size()));
  }
  if (this->_contact.status.size() > m_confManager->get<size_t>("statusMaxSize")
    || this->_contact.status.size() < m_confManager->get<size_t>("statusMinSize"))
  {
    throw(PacketException(PacketAnswer::CONTACT_REQUEST_INVALID_STATUS,
			  "[PacketContactRequest] failed : invalid status length : " + this->_contact.status.size()));
  }

  SerializerBuffer *buffer = new SerializerBuffer;

  buffer->writeToData<uint32_t>(this->_contact.uid);

  buffer->writeToData<uint16_t>(this->_contact.username.size());
  buffer->writeToData(this->_contact.username, this->_contact.username.size());

  buffer->writeToData<uint16_t>(this->_contact.status.size());
  buffer->writeToData(this->_contact.status, this->_contact.status.size());

  return (buffer);
}

void                PacketContactRequest::setUserData(uint32_t uid,
  std::string const &username,
  std::string const &status)
{
  this->_contact.uid = uid;
  this->_contact.username = username;
  this->_contact.status = status;
}

uint32_t            PacketContactRequest::getUserId() const
{
    return (this->_contact.uid);
}

std::string const   &PacketContactRequest:: getUsername() const
{
    return (this->_contact.username);
}

std::string const   &PacketContactRequest:: getStatus() const
{
    return (this->_contact.status);
}
