#pragma once

# include	<list>
# include	<string>
# include	"IService.hh"
# include	"ServiceManager.hh"

class IUserService : public IService
{
public:
  virtual void		addConnectedUser(UserController *userToAdd) = 0;
  virtual bool		delConnectedUser(UserController *userToDel) = 0;
  virtual size_t	countConnectedUsers() const = 0;

  virtual bool				isUserConnected(uint32_t userID) const = 0;
  virtual UserController		*findConnectedUserById(uint32_t idTofind) const = 0;
  virtual std::list<UserController *>	findConnectedUserByUsername(const std::string &usernameToFind) const = 0;

public:
  virtual void		handlePacket(APacket::packetID packetid, SerializerBuffer *buffer, UserController *user) = 0;
  virtual bool		startService(ServiceManager *serviceManager) = 0;
  virtual void		stopService() = 0;
  virtual std::string	getServiceName() const = 0;
  virtual const std::list<APacket::packetID>	&getPacketIds() const = 0;
virtual void onServerEventClientDisconnected(UserController *user) = 0;
virtual void onServerEventClientConnected(UserController *user) = 0;

};
