#ifndef LOGGINSERVICE_H_
# define LOGGINSERVICE_H_

# include	<string>
# include	"../../UserController.hh"
# include	"IService.hh"
# include	"IUserService.hh"
# include	"ServiceManager.hh"
# include	"../database-service/IDatabase.hh"
# include	"ServiceManager.hh"
# include	"../user-service/UserService.hh"

class LogginService : public IService
{
public:
  static const std::string	m_logginServiceName;

private:
  IUserService			*m_userService;
  IDatabase			*m_database;
  std::list<APacket::packetID>	m_packetIds;
  std::map<APacket::packetID, void (LogginService::*)(SerializerBuffer *buffer, UserController *user)>	m_services;

public:
  LogginService();
  ~LogginService();

  void				handlePacket(APacket::packetID packetId, SerializerBuffer *packet,
					     UserController *user);
  bool				startService(ServiceManager *serviceManager);
  void				stopService();
  std::string			getServiceName() const;
  const std::list<APacket::packetID>	&getPacketIds() const;
	void onServerEventClientDisconnected(UserController *user);
	void onServerEventClientConnected(UserController *user);

private:
  void		authentificationHandler(SerializerBuffer *buffer, UserController *user);
  void		registerHandler(SerializerBuffer *buffer, UserController *user);

private:
  LogginService(const LogginService &);
  LogginService &operator=(const LogginService &);
};

#endif /* !LOGGINSERVICE_H_ */
