#pragma once

#include <map>
#include "IDatabase.hh"
#include "UserData.hh"
#include "DatabaseException.hh"
#include <string>
#include	<boost/cstdint.hpp>

class FileDatabase : public IDatabase
{
private:
  const std::string	m_extension;
  const std::string	m_databasePath;
  const std::string	m_storagePath;
  const std::string	m_indexPath;
  const std::string	m_idPath;
  const std::string	m_defaultUsername;
  uint32_t		m_currentMaxId;

  typedef struct	sDataBaseEntry
  {
    std::string		accountName;
    std::string		username;
    uint32_t		id;
    UserData		*userData;
  }			dataBaseEntry;

  std::list<APacket::packetID>	m_packetIds;
  std::map<uint32_t, FileDatabase::dataBaseEntry *>	m_indexById;
  std::map<std::string, FileDatabase::dataBaseEntry *>	m_indexByAccountName;
public:
  FileDatabase();
  ~FileDatabase();

  uint32_t	createAccount(const std::string &accountName,
			      const std::string &hashedPasswd);

  bool		accountExistByName(const std::string &accountName) const;
  bool		accountExistById(uint32_t userId) const;

  void		updateAccount(uint32_t userId, const UserData &userData);

  UserData	getAccountByName(const std::string &accountName);
  UserData	getAccountById(uint32_t userId);
  
const std::vector<UserData>	searchUsersByUsername(const std::string &username);

  void		freeCacheOfId(uint32_t userId);

public:
  void		handlePacket(APacket::packetID packetid, SerializerBuffer *buffer, UserController *user);
  bool		startService(ServiceManager *serviceManager);
  void		stopService();
  std::string	getServiceName() const;
  const std::list<APacket::packetID>	&getPacketIds() const;
 void onServerEventClientDisconnected(UserController *user);
 void onServerEventClientConnected(UserController *user);

private:
  void		initializeIndexes();
  void		updateIndexes();
  void		addEntryInIndexFile(const std::string &accountName);
  void		addEntryInIndexes(const std::string &accountName, const std::string &username, uint32_t id);

  UserData	*readAccountdata(std::ifstream &infile) const;
};
