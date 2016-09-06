#include	"PacketSearchResult.hh"
#include	"PacketAnswer.hh"
#include	"PacketException.hh"

PacketSearchResult::PacketSearchResult()
  : APacket(ID_SEARCH_RESULT, SERVER_TO_CLIENT)
{
}

PacketSearchResult::~PacketSearchResult()
{
}

void  PacketSearchResult::unserialize(SerializerBuffer *inBuffer)
{
  uint16_t	length;

  try
  {
    uint16_t size = inBuffer->readToData<uint16_t>();
    while (size)
      {
        PacketSearchResult::user_t user;

        user.userId = inBuffer->readToData<uint32_t>();

	length = inBuffer->readToData<uint16_t>();
	if (length > m_confManager->get<size_t>("usernameMaxSize") ||
	    length < m_confManager->get<size_t>("usernameMinSize"))
	  throw(PacketException(PacketAnswer::SEARCH_RESULT_INVALID_USERNAME,
				"[PacketSearchResult] failed : invalid username length : " + length));
        user.userName = std::string(inBuffer->readToData(length));

	length = inBuffer->readToData<uint16_t>();
	if (length > m_confManager->get<size_t>("statusMaxSize") ||
	    length < m_confManager->get<size_t>("statusMinSize"))
	  throw(PacketException(PacketAnswer::SEARCH_RESULT_INVALID_STATUS,
				"[PacketSearchResult] failed : invalid status length : " + length));
        user.status = std::string(inBuffer->readToData(length));

        m_userList.push_back(user);
        size--;
      }
  } catch (const SerializerBufferException &e) {
    std::cerr << "[PacketSearchResult] Error : SerializerBufferException failed :"
              << e.what() << std::endl;
  }

}

SerializerBuffer* PacketSearchResult::serialize() const
{
  SerializerBuffer *buffer = new SerializerBuffer;

  buffer->writeToData<uint16_t>(m_userList.size());
  for (std::vector<PacketSearchResult::user_t>::const_iterator it = m_userList.begin();
       it != m_userList.end();
       ++it)
    {
      buffer->writeToData<uint32_t>((*it).userId);


      std::string stringBuffer = (*it).userName;
      if (stringBuffer.size() > m_confManager->get<size_t>("usernameMaxSize") ||
	  stringBuffer.size() < m_confManager->get<size_t>("usernameMinSize"))
	throw(PacketException(PacketAnswer::SEARCH_RESULT_INVALID_USERNAME,
			      "[PacketSearchResult] failed : invalid username length : " + stringBuffer.size()));
      buffer->writeToData<uint16_t>(stringBuffer.size());
      buffer->writeToData(stringBuffer.c_str(), stringBuffer.size());

      stringBuffer = (*it).status;
      if (stringBuffer.size() > m_confManager->get<size_t>("statusMaxSize") ||
	  stringBuffer.size() < m_confManager->get<size_t>("statusMinSize"))
	throw(PacketException(PacketAnswer::SEARCH_RESULT_INVALID_STATUS,
			      "[PacketSearchResult] failed : invalid status length : " + stringBuffer.size()));

      buffer->writeToData<uint16_t>(stringBuffer.size());
      buffer->writeToData(stringBuffer.c_str(), stringBuffer.size());
    }
  return buffer;
}

const std::vector<PacketSearchResult::user_t>	&PacketSearchResult::getUserList() const
{
  return m_userList;
}

void  PacketSearchResult::addUserInlist(uint32_t userId, const std::string &username, const std::string &status)
{
  PacketSearchResult::user_t user;

  user.userId = userId;
  user.userName = username;
  user.status = status;

  m_userList.push_back(user);
}
