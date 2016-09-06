#include    "PacketLoginSuccess.hh"
#include	"PacketAnswer.hh"
#include	"PacketException.hh"

PacketLoginSuccess::PacketLoginSuccess()
    : APacket(APacket::ID_LOGIN_SUCCESS, APacket::SERVER_TO_CLIENT)
{

}

PacketLoginSuccess::~PacketLoginSuccess()
{

}

// ERRATA :
// - Userdata contient une liste de contact, des fonctions d'accès etc.
// - Le packet dir Server -> Client, n'a besoin que de l'uid, de l'username
//  et du status.
// - Pour que le packet ressemble uniquement à un packet,
//  j'ai changer ce que votre fonction contient.
// - Remove l'ERRATA quand vous l'avez lus !.

// void PacketLoginSuccess::setUserData(const UserData &userData)
// {
//   m_userData = userData;
// }

void                PacketLoginSuccess:: setUserData(uint32_t uid,
						     std::string const &username,
						     std::string const &status)
{
    this->_uid = uid;
    this->_username = username;
    this->_status = status;
}

uint32_t            PacketLoginSuccess:: getUserId() const
{
    return (this->_uid);
}

std::string const   &PacketLoginSuccess:: getUsername() const
{
    return (this->_username);
}

std::string const   &PacketLoginSuccess:: getStatus() const
{
    return (this->_status);
}

// Server -> Client, Used on client side.
void                PacketLoginSuccess::unserialize(SerializerBuffer *in)
{
    uint16_t        size;

    try {
        this->_uid = in->readToData<uint32_t>();
        size = in->readToData<uint16_t>();
    	this->_username = std::string(in->readToData(size));
        size = in->readToData<uint16_t>();
        this->_status = std::string(in->readToData(size));
    } catch (SerializerBufferException const &e) {
        std::cerr << "[PacketLoginSuccess] Error : " << e.what() << "." << std::endl;
    }
}

SerializerBuffer    *PacketLoginSuccess::serialize() const
{
  if (_username.size() > m_confManager->get<size_t>("usernameMaxSize") ||
      _username.size() < m_confManager->get<size_t>("usernameMinSize"))
    throw(PacketException(PacketAnswer::LOGIN_SUCCESS_INVALID_USERNAME,
			  "[PacketLoginSuccess] failed : invalid status length : " + _username.size()));

  if (_status.size() > m_confManager->get<size_t>("statusMaxSize") ||
      _status.size() < m_confManager->get<size_t>("statusMinSize"))
    throw(PacketException(PacketAnswer::LOGIN_SUCCESS_INVALID_STATUS,
			  "[PacketLoginSuccess] failed : invalid status length : " + _username.size()));

  SerializerBuffer *buffer = new SerializerBuffer;

    buffer->writeToData<uint32_t>(this->_uid);

    buffer->writeToData<uint16_t>(this->_username.size());
    buffer->writeToData(this->_username.c_str(), this->_username.size());

    buffer->writeToData<uint16_t>(this->_status.size());
    buffer->writeToData(this->_status.c_str(), this->_status.size());

    return (buffer);
}
