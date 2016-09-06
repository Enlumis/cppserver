#pragma once

# include	<map>
# include	<list>
# include	<vector>
# include	"IService.hh"
# include	"../../UserController.hh"
# include	"../ServiceManager.hh"
# include	"../IUserService.hh"
# include	"../IChannelService.hh"
# include	"../database-service/IDatabase.hh"
# include	"CallGroup.hh"
# include	"../channel-service/Room.hh"

class CallService : public IService
{
private:
  std::list<APacket::packetID>	m_packetIds;
  std::map<APacket::packetID, void (CallService::*)(SerializerBuffer *buffer, UserController *user)>	m_services;

  IUserService			*m_userService;
  IChannelService		*m_channelService;
  IDatabase			*m_databaseService;

  std::vector<CallGroup>	m_callGroups;

public:
  CallService();
  ~CallService();

  void			handlePacket(APacket::packetID packetid, SerializerBuffer *buffer, UserController *user);
  bool				startService(ServiceManager *serviceManager);
  void				stopService();
  std::string			getServiceName() const;
  const std::list<APacket::packetID>	&getPacketIds() const;

  void		onServerEventClientDisconnected(UserController *user);
  void		onServerEventClientConnected(UserController *user);

  CallGroup	&getOrCreateCallGroup(uint32_t hostUser);
  void		removeCallGroup(uint32_t hostUser);
  bool		callGroupExistsByHostUser(uint32_t hostUser) const;

private:
  void		call(SerializerBuffer *buffer, UserController *user);
  void		callJoin(SerializerBuffer *buffer, UserController *user);
  void		callQuit(SerializerBuffer *buffer, UserController *user);

private:
  CallService(const CallService &);
  CallService &operator=(const CallService &);
};
