#pragma once

#include	<map>
#include	<string>
#include	"IService.hh"
#include	"UserData.hh"
#include	"DatabaseException.hh"

class IDatabase : public IService
{
public:
  IDatabase() {};
  virtual ~IDatabase() {}

  virtual uint32_t	createAccount(const std::string &accountName,
				 const std::string &hashedPasswd) = 0;

  virtual bool		accountExistByName(const std::string &accountName) const = 0;
  virtual bool		accountExistById(uint32_t userId) const = 0;

  virtual void		updateAccount(uint32_t userId, const UserData &userData) = 0;

  virtual UserData	getAccountByName(const std::string &accountName) = 0;
  virtual UserData	getAccountById(uint32_t userId) = 0;
  virtual const std::vector<UserData>	searchUsersByUsername(const std::string &username) = 0;

  virtual void		freeCacheOfId(uint32_t userId) = 0;
public:

  virtual void		handlePacket(APacket::packetID packetid, SerializerBuffer *buffer,
				     UserController *user) = 0;
  virtual bool		startService(ServiceManager *serviceManager) = 0;
  virtual void		stopService() = 0;
  virtual std::string	getServiceName() const = 0;
  virtual const std::list<APacket::packetID>	&getPacketIds() const = 0;
virtual void onServerEventClientDisconnected(UserController *user) = 0;
virtual void onServerEventClientConnected(UserController *user) = 0;
};
