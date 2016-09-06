#include "PacketOpenChannel.hh"
#include  <vector>
#include  "PacketAnswer.hh"
#include  "PacketException.hh"

PacketOpenChannel::PacketOpenChannel()
  : APacket(APacket::ID_OPEN_CHANNEL, APacket::SERVER_TO_CLIENT)
{
}

PacketOpenChannel::~PacketOpenChannel()
{}

void  PacketOpenChannel::unserialize(SerializerBuffer *in)
{
  try {
    _channelId = in->readToData<uint32_t>();
    uint16_t        size = in->readToData<uint16_t>();

    size_t          idx = 0;
    while (idx < size)
      {
        size_t          nb;
        Type::UserField tmp;

        tmp.uid = in->readToData<uint32_t>();

        nb = in->readToData<uint16_t>();
        if (nb > m_confManager->get<size_t>("usernameMaxSize")
          || nb < m_confManager->get<size_t>("usernameMinSize"))
        {
          throw PacketException(PacketAnswer::CONTACT_LIST_INVALID_USERNAME
            , "[PacketContactList] failed : invalid username length : " + nb);
        }

        tmp.username = std::string(in->readToData(nb));

        nb = in->readToData<uint16_t>();
        if (nb > m_confManager->get<size_t>("statusMaxSize")
          || nb < m_confManager->get<size_t>("statusMinSize"))
        {
          throw PacketException(PacketAnswer::CONTACT_LIST_INVALID_STATUS
            , "[PacketContactList] failed : invalid status length : " + nb);
        }
        tmp.status = std::string(in->readToData(nb));

        this->_users.push_back(tmp);
        idx += 1;
      }
  } catch (const SerializerBufferException &e) {
    std::cerr   << "[PacketOpenChannel] Error : SerializerBufferException failed : "
                << e.what() << std::endl;
  }
}

SerializerBuffer*  PacketOpenChannel::serialize() const
{
  SerializerBuffer *out = new SerializerBuffer;

  out->writeToData<uint32_t>(_channelId);


  auto it = this->_users.begin();
  out->writeToData<uint16_t>(this->_users.size());
  while (it != this->_users.end())
    {
      if (it->username.length() > m_confManager->get<size_t>("usernameMaxSize")
        || it->username.length() < m_confManager->get<size_t>("usernameMinSize"))
      {
        throw PacketException(PacketAnswer::CONTACT_REQUEST_INVALID_USERNAME
          , "[PacketOpenChannel] failed : invalid username length : " + it->username.length());
      }
      if (it->status.length() > m_confManager->get<size_t>("statusMaxSize")
        || it->status.length() < m_confManager->get<size_t>("statusMinSize"))
      {
        throw PacketException(PacketAnswer::CONTACT_REQUEST_INVALID_STATUS
          , "[PacketOpenChannel] failed : invalid status length : " + it->status.length());
      }
      out->writeToData<uint32_t>(it->uid);
      out->writeToData<uint16_t>(it->username.length());
      out->writeToData(it->username, it->username.length());
      out->writeToData<uint16_t>(it->status.length());
      out->writeToData(it->status, it->status.length());
      ++it;
    }
  return out;
}

uint32_t PacketOpenChannel::getChannelId() const
{
  return _channelId;
}

void PacketOpenChannel::setChannelId(const uint32_t &channelId)
{
  _channelId = channelId;
}

std::vector<Type::UserField> const &PacketOpenChannel:: getChannelUsers() const
{
  return (this->_users);
}

void  PacketOpenChannel:: setUserdata(uint32_t id, std::string const &username, std::string const &status)
{
  Type::UserField tmp;

  tmp.uid = id;
  tmp.username = username;
  tmp.status = status;
  this->_users.push_back(tmp);
}