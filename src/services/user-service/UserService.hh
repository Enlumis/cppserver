#ifndef REGISTERSERVICE_H_
# define REGISTERSERVICE_H_

# include	<list>
# include	<string>
# include	"IUserService.hh"
# include	"ServiceManager.hh"
# include	"../database-service/IDatabase.hh"

class UserService : public IUserService
{
public:
  static const std::string	m_userServiceName;

private:
  IDatabase			*m_database;
  std::list<UserController *>	m_connectedUsers;
  std::list<APacket::packetID>	m_packetIds;
  std::map<APacket::packetID, void (UserService::*)(SerializerBuffer *buffer,
						    UserController *user)>	m_services;

public:
  UserService();
  ~UserService() {}

  void				handlePacket(APacket::packetID packetId, SerializerBuffer *packet,
					     UserController *user);
  bool				startService(ServiceManager *serviceManager);
  void				stopService();
  std::string			getServiceName() const;
  const std::list<APacket::packetID>	&getPacketIds() const;

  void				onServerEventClientDisconnected(UserController *user);
  void				onServerEventClientConnected(UserController *user);

  void				addConnectedUser(UserController *userToAdd);
  bool				delConnectedUser(UserController *userToDel);
  size_t			countConnectedUsers() const;

private:
  void				searchUser(SerializerBuffer *buffer, UserController *user);
  void				updateUserName(SerializerBuffer *buffer, UserController *user);
  void				updateStatus(SerializerBuffer *buffer, UserController *user);
  void				addContact(SerializerBuffer *buffer, UserController *user);
  void				refuseContact(SerializerBuffer *buffer, UserController *user);

  bool				isUserConnected(uint32_t userID) const;
  UserController		*findConnectedUserById(uint32_t idTofind) const;
  std::list<UserController *>	findConnectedUserByUsername(const std::string &usernameToFind) const;

private:
  UserService(const UserService &);
  UserService &operator=(const UserService &);

};

#endif /* !REGISTERSERVICE_H_ */
