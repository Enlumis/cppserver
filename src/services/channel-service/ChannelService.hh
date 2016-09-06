#pragma once

# include	<map>
# include	<list>
# include	"../IChannelService.hh"
# include	"../ServiceManager.hh"
# include	"../database-service/IDatabase.hh"
# include	"../user-service/UserService.hh"
# include	"Room.hh"

class ChannelService : public IChannelService
{
private:
  uint32_t			m_currentMaxChannelId;
  IDatabase			*m_databaseService;
  IUserService			*m_userService;
  std::list<APacket::packetID>	m_packetIds;
  std::map<APacket::packetID, void (ChannelService::*)(SerializerBuffer *buffer, UserController *user)>	m_services;

private:
  std::map<uint32_t, Room>	m_channels;

public:
  ChannelService();
  ~ChannelService();

  void		handlePacket(APacket::packetID packetid, SerializerBuffer *buffer, UserController *user);
  bool				startService(ServiceManager *serviceManager);
  void				stopService();
  std::string			getServiceName() const;
  const std::list<APacket::packetID>	&getPacketIds() const;

  void onServerEventClientDisconnected(UserController *user);
  void onServerEventClientConnected(UserController *user);

  const Room	&findRoomById(uint32_t roomId) const;
  const bool	roomExistsById(uint32_t roomId) const;
  const std::map<uint32_t, Room>	&getRooms() const;

private:
  void		createChannel(SerializerBuffer *buffer, UserController *user);
  void		addUserToChannel(SerializerBuffer *buffer, UserController *user);
  void		sendMessage(SerializerBuffer *buffer, UserController *user);

private:
  ChannelService(const ChannelService &);
  ChannelService &operator=(const ChannelService &);
};
