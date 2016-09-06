#include	"PacketChannelEvent.hh"
#include	"PacketAnswer.hh"
#include	"PacketException.hh"

PacketChannelEvent::PacketChannelEvent()
  : APacket(APacket::ID_CHANNEL_EVENT, APacket::SERVER_TO_CLIENT)
{
}

PacketChannelEvent::~PacketChannelEvent()
{}

void              PacketChannelEvent::unserialize(SerializerBuffer *inBuffer)
{
  try {
    _channelId = inBuffer->readToData<uint32_t>();
    _idEvent = inBuffer->readToData<uint8_t>();
    _user.uid = inBuffer->readToData<uint32_t>();

    uint16_t length = inBuffer->readToData<uint16_t>();
    if (length > m_confManager->get<int>("usernameMaxSize") ||
	length < m_confManager->get<int>("usernameMinSize"))
      throw(PacketException(PacketAnswer::UPDATE_USERNAME_INVALID,
			    "PacketChannelEvent failed : invalid username length : " + length));
    _user.username = std::string(inBuffer->readToData(length));

    length = inBuffer->readToData<uint16_t>();
    if (length > m_confManager->get<int>("statusMaxSize") ||
	length < m_confManager->get<int>("statusMinSize"))
      throw(PacketException(PacketAnswer::UPDATE_USERNAME_INVALID,
			    "PacketChannelEvent failed : invalid status length : " + length));
    _user.status = std::string(inBuffer->readToData(length));

  } catch (const SerializerBufferException &e) {
    std::cerr   << "[PacketChannelEvent] Error : SerializerBufferException failed : "
                << e.what() << std::endl;
  }
}

SerializerBuffer  *PacketChannelEvent::serialize() const
{
  uint16_t usernameSize = _user.username.size();
  if (usernameSize > m_confManager->get<int>("usernameMaxSize") ||
      usernameSize < m_confManager->get<int>("usernameMinSize"))
    throw(PacketException(PacketAnswer::UPDATE_USERNAME_INVALID,
			  "PacketChannelEvent failed : invalid username length : " + usernameSize));
  uint16_t statusSize = _user.status.size();
  if (statusSize > m_confManager->get<int>("statusMaxSize") ||
      statusSize < m_confManager->get<int>("statusMinSize"))
    throw(PacketException(PacketAnswer::UPDATE_USERNAME_INVALID,
			  "PacketChannelEvent failed : invalid status length : " + statusSize));

  SerializerBuffer *buffer = new SerializerBuffer();

  buffer->writeToData<uint32_t>(_channelId);
  buffer->writeToData<uint8_t>(_idEvent);
  buffer->writeToData<uint32_t>(_user.uid);
  buffer->writeToData<uint16_t>(usernameSize);
  buffer->writeToData(_user.username.c_str(), usernameSize);
  buffer->writeToData<uint16_t>(statusSize);
  buffer->writeToData(_user.status.c_str(), statusSize);
  return buffer;
}

uint32_t PacketChannelEvent::getChannelId() const
{
  return _channelId;
}

void PacketChannelEvent::setChannelId(const uint32_t &channelId)
{
  _channelId = channelId;
}

uint8_t	PacketChannelEvent::getIdEvent() const
{
  return _idEvent;
}

void		PacketChannelEvent::setIdEvent(uint8_t event)
{
  _idEvent = event;
}

uint32_t PacketChannelEvent::getUserId() const
{
  return _user.uid;
}

void PacketChannelEvent::setUserId(const uint32_t &userId)
{
  _user.uid = userId;
}

const std::string	&PacketChannelEvent::getUsername() const
{
  return _user.username;
}

void			PacketChannelEvent::setUsername(const std::string &username)
{
  _user.username = username;
}

const std::string	&PacketChannelEvent::getStatus() const
{
  return _user.status;
}

void			PacketChannelEvent::setStatus(const std::string &status)
{
  _user.status = status;
}

const Type::UserField	&PacketChannelEvent::getUser()
{
  return _user;
}

void			PacketChannelEvent::setUser(Type::UserField const &user)
{
  _user = user;
}
