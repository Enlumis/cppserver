# include       "PacketContactList.hh"
#include	"PacketAnswer.hh"
#include	"PacketException.hh"

PacketContactList::     PacketContactList()
  : APacket(APacket::ID_CONTACT_LIST, APacket::SERVER_TO_CLIENT)
{

}

PacketContactList::     ~PacketContactList()
{

}

SerializerBuffer        *PacketContactList:: serialize() const
{
  SerializerBuffer    *out = new SerializerBuffer();

  std::vector<Type::UserField>::const_iterator it = this->_contacts.begin();

  out->writeToData<uint16_t>(this->_contacts.size());
  while (it != this->_contacts.end())
    {
      if (it->username.length() > m_confManager->get<size_t>("usernameMaxSize")
        || it->username.length() < m_confManager->get<size_t>("usernameMinSize"))
      {
        throw PacketException(PacketAnswer::CONTACT_REQUEST_INVALID_USERNAME
          , "[PacketContactRequest] failed : invalid username length : " + it->username.length());
      }
      if (it->status.length() > m_confManager->get<size_t>("statusMaxSize")
        || it->status.length() < m_confManager->get<size_t>("statusMinSize"))
      {
        throw PacketException(PacketAnswer::CONTACT_REQUEST_INVALID_STATUS
          , "[PacketContactRequest] failed : invalid status length : " + it->status.length());
      }
      out->writeToData<uint32_t>(it->uid);
      out->writeToData<uint16_t>(it->username.length());
      out->writeToData(it->username, it->username.length());
      out->writeToData<uint16_t>(it->status.length());
      out->writeToData(it->status, it->status.length());
      ++it;
    }
  return (out);
}

void                    PacketContactList:: unserialize(SerializerBuffer *in)
{
  try {
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

        this->_contacts.push_back(tmp);
        idx += 1;
      }
  } catch (SerializerBufferException &e) {
    std::cerr << "[PacketContactList] Error : " << e.what() << std::endl;
  }
}

void                    PacketContactList:: addContact(uint32_t uid,
						       std::string const &username,
						       std::string const &status)
{
  Type::UserField  tmp;

  tmp.uid = uid;
  tmp.username = username;
  tmp.status = status;

  this->_contacts.push_back(tmp);
}

std::vector<Type::UserField> const &PacketContactList:: getContacts() const
{
  return (this->_contacts);
}
