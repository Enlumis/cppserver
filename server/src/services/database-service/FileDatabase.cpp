#include <fstream>
#include <iostream>
#include "FileDatabase.hh"
#include "FileHelper.hh"
#include "../packets/SerializerBuffer.hpp"

FileDatabase::FileDatabase()
  : m_extension("babeldata"),
    m_databasePath("babel-database/"),
    m_storagePath(m_databasePath + "users/"),
    m_indexPath(m_databasePath + "users_index." + m_extension),
    m_idPath(m_databasePath + "id." + m_extension),
    m_defaultUsername("changeYouUsername"),
    m_currentMaxId(0)
{
}

FileDatabase::~FileDatabase()
{
  std::cout << "[FileDatabase] Close database..." << std::endl;
}
void	FileDatabase::handlePacket(APacket::packetID packetid, SerializerBuffer *buffer, UserController *user)
{}

void FileDatabase::onServerEventClientDisconnected(UserController *user)
{
  UserData *userData = user->getDataContainer().getData<UserData>("UserData");
  if (!userData)
    return ;
  freeCacheOfId(userData->getId());
}

void FileDatabase::onServerEventClientConnected(UserController *user)
{}

bool	FileDatabase::	startService(ServiceManager *serviceManager)
{
  std::cout << "[FileDatabase] Initialize database" << std::endl;
  std::cout << "[FileDatabase] databasePath : '" << m_databasePath << "' indexPath : '" << m_indexPath << "'" << std::endl;

  if (!FileHelper::directoryExist(m_databasePath))
    {
      std::cout << "[FileDatabase] database directory doesn't exist, creating it..." << std::endl;
      if (!FileHelper::createDirectory(m_databasePath)){
	std::cout << "[FileDatabase] Fail to create database directory '" + m_databasePath + "'" << std::endl;
      }
    }
  if (!FileHelper::directoryExist(m_storagePath))
    {
      std::cout << "[FileDatabase] database users directory doesn't exist, creating it..." << std::endl;
      if (!FileHelper::createDirectory(m_storagePath)){
	std::cout << "[FileDatabase] Fail to create database users directory '" + m_storagePath + "'" << std::endl;
      }
    }
  if (!FileHelper::fileExist(m_indexPath))
    {
      std::ofstream outfile (m_indexPath.c_str(), std::ios::binary);
      if (!outfile.is_open() || !outfile.good())
	{
	  std::cout << "[FileDatabase] Fail to create index file '" + m_indexPath + "'" << std::endl;
	  return false;
	}
      outfile.close();
    }

  initializeIndexes();
  return true;
}

void	FileDatabase::	stopService()
{
  for (std::map<uint32_t, FileDatabase::dataBaseEntry *>::iterator it = m_indexById.begin();
       it != m_indexById.end();
       ++it)
    {
      delete (*it).second;
    }
}

std::string	FileDatabase::getServiceName() const
{
  return "DatabaseService";
}

const std::list<APacket::packetID>	&FileDatabase::getPacketIds() const
{
  return m_packetIds;
}

void FileDatabase::updateIndexes()
{
  std::ofstream outfile (m_indexPath.c_str(), std::ios::binary);

  for (std::map<uint32_t, FileDatabase::dataBaseEntry *>::iterator it = m_indexById.begin();
       it != m_indexById.end();
       ++it)
    {
      SerializerBuffer buffer;
      buffer.writeToData<uint32_t>((*it).second->accountName.size());
      buffer.writeToData((*it).second->accountName, (*it).second->accountName.size());

      buffer.writeToData<uint32_t>((*it).second->username.size());
      buffer.writeToData((*it).second->username, (*it).second->username.size());

      buffer.writeToData<uint32_t>((*it).second->id);

      int bufferSize = buffer.size();
      std::string dataSerialized = buffer.readToData(bufferSize);
      outfile.write(dataSerialized.c_str(), bufferSize);
    }
  outfile.close();
}

uint32_t	FileDatabase::createAccount(const std::string &accountName,
					    const std::string &hashedPasswd)
{
  std::string name(this->m_storagePath);

  if (m_currentMaxId == 0)
    throw DatabaseException("[ FileDatabase ] Error : current max id not setted !");

  name += accountName + "." + m_extension;
  std::ofstream outfile (name.c_str(), std::ios::binary);
  if (!outfile.is_open() || !outfile.good())
    throw DatabaseException("Fail to create userdata '" + accountName + "', rootPath have write permission ?");
  outfile.close();

  addEntryInIndexFile(accountName);

  UserData userData;
  userData.setId(m_currentMaxId);
  userData.setUsername(m_defaultUsername);
  userData.setHashedPasswd(hashedPasswd);
  userData.setStatus("You just registered, how do you feel ?");

  updateAccount(m_currentMaxId, userData);

  std::cout << "[FileDatabase] : created User : " << accountName << std::endl;

  return (m_currentMaxId++);
}

bool	FileDatabase::accountExistByName(const std::string &accountName) const
{
  std::map<std::string, FileDatabase::dataBaseEntry *>::const_iterator it = m_indexByAccountName.find(accountName);
  return (it != m_indexByAccountName.end());
}

bool	FileDatabase::accountExistById(uint32_t userId) const
{
  std::map<uint32_t, FileDatabase::dataBaseEntry *>::const_iterator it = m_indexById.find(userId);
  return (it != m_indexById.end());
}

UserData	FileDatabase::getAccountByName(const std::string &accountName)
{
  std::string name(this->m_storagePath);

  std::map<std::string, FileDatabase::dataBaseEntry *>::iterator it = m_indexByAccountName.find(accountName);
  if (it == m_indexByAccountName.end())
    throw DatabaseException("Error : accountName not found : " + accountName);
  FileDatabase::dataBaseEntry *entry = it->second;

  if (entry->userData != NULL)
    return *(entry->userData);

  name += accountName + "." + m_extension;
  std::ifstream infile(name.c_str(), std::ios::binary);

  if (!infile)
    throw DatabaseException("Fail to get userdata '" + accountName + "', data exists ?");

  try {
    entry->userData = readAccountdata(infile);
  } catch (const SerializerBufferException &e){
    throw DatabaseException("Fail to read userdata '" + accountName + "', data is probably corrupted");
  }
  std::cout << "[FileDatabase] Loaded User : " << accountName
	    << ", " << entry->userData->getUsername()
	    << ", contacts(" << entry->userData->getContacts().size() << ")"
	    << ", contactsRequest(" << entry->userData->getContactsRequest().size() << ")"
	    << std::endl;

  infile.close();
  return *(entry->userData);
}

UserData	FileDatabase::getAccountById(uint32_t userId)
{
  std::string name(this->m_storagePath);

  std::map<uint32_t, FileDatabase::dataBaseEntry *>::iterator it = m_indexById.find(userId);
  if (it == m_indexById.end())
    throw DatabaseException("Error : userID not found : " + userId);
  FileDatabase::dataBaseEntry *entry = it->second;

  if (entry->userData != NULL)
    return *(entry->userData);

  name += entry->accountName + "." + m_extension;
  std::ifstream infile(name.c_str(), std::ios::binary);

  if (!infile)
    throw DatabaseException("Fail to get userdata '" + entry->accountName + "', data exists ?");

  try {
    entry->userData = readAccountdata(infile);
  } catch (const SerializerBufferException &e){
    throw DatabaseException("Fail to read userdata '" + entry->accountName + "', data is probably corrupted");
  }
  std::cout << "[FileDatabase] Loaded User : " << entry->accountName
	    << ", " << entry->userData->getUsername()
	    << ", contacts(" << entry->userData->getContacts().size() << ")"
	    << ", contactsRequest(" << entry->userData->getContactsRequest().size() << ")"
	    << std::endl;

  infile.close();
  return *(entry->userData);
}

const std::vector<UserData>	FileDatabase::searchUsersByUsername(const std::string &username)
{
  std::vector<UserData> vecUsers;
  for (std::map<uint32_t, FileDatabase::dataBaseEntry *>::iterator it = m_indexById.begin();
       it != m_indexById.end();
       ++it)
    {
      std::cout << "[FileDatabase::searchUsersByUsername] : -> Search user : "
		<< (*it).second->username << std::endl;
      if ((*it).second->username.find(username) != std::string::npos)
	{
	  std::cout << "[FileDatabase::searchUsersByUsername] : Search user : SUCCESS " << std::endl;
	  if ((*it).second->userData != NULL){
	    vecUsers.push_back(*((*it).second->userData));
	  }else{
	    UserData data;
	    data.setId((*it).first);
	    data.setUsername((*it).second->username);
		data.setStatus("Offline");
	    vecUsers.push_back(data);
	  }
	}
    }
  return vecUsers;
}

UserData	*FileDatabase::readAccountdata(std::ifstream &infile) const
{
  UserData *data = new UserData;

  SerializerBuffer buffer;
  infile.seekg (0, infile.end);
  int length = infile.tellg();
  infile.seekg (0, infile.beg);
  char *contents = new char [length];
  infile.read(contents, length);
  buffer.writeToData(contents, length);

  data->setId(buffer.readToData<uint32_t>());

  length = buffer.readToData<int>();
  data->setUsername(buffer.readToData(length));

  length = buffer.readToData<int>();
  data->setHashedPasswd(buffer.readToData(length));

  length = buffer.readToData<int>();
  data->setStatus(buffer.readToData(length));

  int nbContact = buffer.readToData<int>();
  for (int i = 0; i < nbContact; ++i)
    data->addContact(buffer.readToData<uint32_t>());

  int nbContactRequest = buffer.readToData<int>();
  for (int i = 0; i < nbContactRequest; ++i)
    data->addContactRequest(buffer.readToData<uint32_t>());
  return data;
}

void	FileDatabase::updateAccount(uint32_t userId, const UserData &userData)
{
  SerializerBuffer buffer;
  std::string name(this->m_storagePath);

  if (!accountExistById(userId))
    throw DatabaseException("Error : userID not found : " + userId);


  FileDatabase::dataBaseEntry *entry = m_indexById[userId];

  name += entry->accountName + "." + m_extension;

  buffer.writeToData<uint32_t>(userData.getId());

  std::string username = userData.getUsername();
  buffer.writeToData<int>(username.size());
  buffer.writeToData(username, username.size());

  std::string hashedPasswd = userData.getHashedPasswd();
  buffer.writeToData<int>(hashedPasswd.size());
  buffer.writeToData(hashedPasswd, hashedPasswd.size());

  std::string status = userData.getStatus();
  buffer.writeToData<int>(status.size());
  buffer.writeToData(status, status.size());

  const std::vector<uint32_t> &contacts = userData.getContacts();
  buffer.writeToData<int>(contacts.size());
  for (int i = 0; i < contacts.size(); ++i)
    buffer.writeToData<uint32_t>(contacts[i]);

  const std::vector<uint32_t> &contactsRequest = userData.getContactsRequest();
  buffer.writeToData<int>(contactsRequest.size());
  for (int i = 0; i < contactsRequest.size(); ++i)
    buffer.writeToData<uint32_t>(contactsRequest[i]);

  std::ofstream outfile (name.c_str(), std::ios::binary);
  int bufferSize = buffer.size();
  std::string dataSerialized = buffer.readToData(bufferSize);

  if (!outfile.is_open() || !outfile.good())
    throw DatabaseException("Fail to write userdata '" + entry->accountName + "', rootPath has write permission ?");

  outfile.write(dataSerialized.c_str(), bufferSize);
  outfile.close();

  bool needUpdateIndexes = false;
  if (userData.getUsername() != entry->username)
    needUpdateIndexes = true;
  entry->username = userData.getUsername();

  UserData *data = entry->userData;
  if (data != NULL)
    {
      data->setUsername(userData.getUsername());
      data->setStatus(userData.getStatus());
      data->setHashedPasswd(userData.getHashedPasswd());
      data->setContact(userData.getContacts());
      data->setContactRequest(userData.getContactsRequest());

      if (needUpdateIndexes)
	this->updateIndexes();
    }

  std::cout << "[FileDatabase] : User updated : " << entry->accountName
	    << " - (userData) :" << userData.getUsername()
	    << ", contact count = " << contacts.size()
	    << ", contactRequests count = " << contactsRequest.size() << std::endl;
}

void	FileDatabase::addEntryInIndexFile(const std::string &accountName)
{
  std::ofstream outfile (m_indexPath.c_str(), std::ios::app);

  SerializerBuffer buffer;
  buffer.writeToData<uint32_t>(accountName.size());
  buffer.writeToData(accountName, accountName.size());

  buffer.writeToData<uint32_t>(m_defaultUsername.size());
  buffer.writeToData(m_defaultUsername, m_defaultUsername.size());

  buffer.writeToData<uint32_t>(m_currentMaxId);

  int bufferSize = buffer.size();
  std::string dataSerialized = buffer.readToData(bufferSize);
  outfile.write(dataSerialized.c_str(), bufferSize);
  outfile.close();
  std::cout << "indexing : " << accountName << std::endl;
  addEntryInIndexes(accountName, m_defaultUsername, m_currentMaxId);
}

void		FileDatabase::freeCacheOfId(uint32_t userId)
{
  std::map<uint32_t, FileDatabase::dataBaseEntry *>::iterator it = m_indexById.find(userId);
  if (it == m_indexById.end())
    return ;
  FileDatabase::dataBaseEntry *entry = it->second;
  delete entry->userData;
  entry->userData = NULL;
}

void	FileDatabase::addEntryInIndexes(const std::string &accountName, const std::string &username, uint32_t id)
{
  FileDatabase::dataBaseEntry *newEntry = new FileDatabase::dataBaseEntry;
  newEntry->accountName = accountName;
  newEntry->username = username;
  newEntry->id = id;
  newEntry->userData = NULL;

  m_indexById[m_currentMaxId] = newEntry;
  m_indexByAccountName[accountName] = newEntry;
}

void	FileDatabase::initializeIndexes()
{
  std::ifstream infile(m_indexPath.c_str(), std::ios::binary);

  if (!infile)
    throw DatabaseException("Fail to read index '" + m_indexPath + "', data is probably corrupted");

  std::cout << "[FileDatabase] Parse index file '" << m_indexPath << "'" << std::endl;
  SerializerBuffer buffer;
  infile.seekg (0, infile.end);
  int length = infile.tellg();
  infile.seekg (0, infile.beg);
  char *contents = new char [length];
  infile.read(contents, length);
  buffer.writeToData(contents, length);
  try {
    std::string	accountName;
    std::string	username;
    uint32_t	lastId = 0;
    uint32_t	length;

    while (buffer.size() > 0)
      {
	length = buffer.readToData<uint32_t>();
	accountName = buffer.readToData(length);

	length = buffer.readToData<uint32_t>();
	username = buffer.readToData(length);

	m_currentMaxId = buffer.readToData<uint32_t>();

	if (lastId > 0 && lastId >= m_currentMaxId)
	  throw DatabaseException("Error : index file '" + m_indexPath +  "' is corrupted");
	addEntryInIndexes(accountName, username, m_currentMaxId);
	lastId = m_currentMaxId;
      }
  } catch (const SerializerBufferException &e){
    m_currentMaxId = 0;
    throw DatabaseException("Fail to read index '" + m_indexPath + "', data is probably corrupted");
  }
  infile.close();
  m_currentMaxId += 1;
}
