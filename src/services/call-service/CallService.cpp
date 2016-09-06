#include	<iostream>
#include	"CallService.hh"
#include	"../../UserController.hh"
#include	"../../packets/PacketAnswer.hh"
#include	"../../packets/PacketException.hh"
#include	"../../services/UserData.hh"
#include	"../../packets/PacketCall.hh"
#include	"../../packets/PacketOnCall.hh"
#include	"../../packets/PacketCallJoin.hh"
#include	"../../packets/PacketCallOnJoin.hh"
#include	"../../packets/PacketCallOnQuit.hh"
#include	"../../packets/PacketCallQuit.hh"
#include	"../../packets/PacketCallStop.hh"

CallService::CallService()
{
  m_packetIds.push_front(APacket::ID_CALL);
  m_packetIds.push_front(APacket::ID_CALL_JOIN);
  m_packetIds.push_front(APacket::ID_CALL_QUIT);

  m_services[APacket::ID_CALL] = &CallService::call;
  m_services[APacket::ID_CALL_JOIN] = &CallService::callJoin;
  m_services[APacket::ID_CALL_QUIT] = &CallService::callQuit;
}

CallService::~CallService()
{

}

void CallService::onServerEventClientDisconnected(UserController *user)
{

}
void CallService::onServerEventClientConnected(UserController *user)
{
}

void	CallService::handlePacket(APacket::packetID packetid, SerializerBuffer *buffer, UserController *user)
{
  std::cout << "[ CallService ] : executeTask..." << std::endl;
  if (!user->isAuthentificated())
  {
	  std::cout << "[ CallService ] : warning an user is not authenticate and he try to communicate with unauthorized service..." << std::endl;
	  user->disconnect();
  }
  void (CallService::*service)(SerializerBuffer *buffer, UserController *user);

  if ((service = m_services[packetid]))
    (this->*service)(buffer, user);
}

bool	CallService::	startService(ServiceManager *serviceManager)
{
  IService *userService = serviceManager->getService("UserService");
  if (!userService || !(m_userService = dynamic_cast<IUserService *>(userService)))
    {
      std::cout << "[ " << this->getServiceName() << " ]  : "
		<< "Warning, user service not found." << std::endl;
      return false;
    }

  IService *channelService = serviceManager->getService("ChannelService");
  if (!channelService || !(m_channelService = dynamic_cast<IChannelService *>(channelService)))
    {
      std::cout << "[ " << this->getServiceName() << " ]  : "
		<< "Warning, channel service not found." << std::endl;
      return false;
    }

  IService *databaseService = serviceManager->getService("DatabaseService");
  if (!databaseService || !(m_databaseService = dynamic_cast<IDatabase *>(databaseService)))
    {
      std::cout << "[ " << this->getServiceName() << " ]  : "
		<< "Warning, DatabaseService not found." << std::endl;
      return false;
    }

  return true;
}

void	CallService::	stopService()
{

}

std::string	CallService::getServiceName() const
{
  return "CallService";
}

const std::list<APacket::packetID>	&CallService::getPacketIds() const
{
  return m_packetIds;
}

void		CallService::removeCallGroup(uint32_t hostUser)
{
  for (std::vector<CallGroup>::iterator it = m_callGroups.begin();
       it != m_callGroups.end();
       ++it)
    {
      if (it->getHostUserId() == hostUser)
	{
	  m_callGroups.erase(it);
	  return ;
	}
    }
}

CallGroup	&CallService::getOrCreateCallGroup(uint32_t hostUser)
{
  for (std::vector<CallGroup>::iterator it = m_callGroups.begin();
       it != m_callGroups.end();
       ++it)
    {
      if (it->getHostUserId() == hostUser)
	{
	  return (*it);
	}
    }

  std::cout << "[CallService::getOrCreateCallGroup] new callGroup created. host user id is '"
	    << hostUser << "'." << std::endl;
  size_t index = m_callGroups.size() + 1;
  m_callGroups.resize(index);
  m_callGroups[index - 1].setHostUserId(hostUser);
  return m_callGroups[index - 1];
}

bool		CallService::callGroupExistsByHostUser(uint32_t hostUser) const
{
  for (std::vector<CallGroup>::const_iterator it = m_callGroups.begin();
       it != m_callGroups.end();
       ++it)
    {
      if (it->getHostUserId() == hostUser)
	{
	  return true;
	}
    }
  return false;
}

void		CallService::call(SerializerBuffer *buffer, UserController *user)
{
  PacketCall packetCall;
  packetCall.unserialize(buffer);

  UserData *userData = user->getDataContainer().getData<UserData>("UserData");

  uint32_t netUserId = packetCall.getUserId();
  UserController *netUserController = m_userService->findConnectedUserById(netUserId);

  uint32_t roomId = packetCall.getRoomId();
  bool isPublicCall = static_cast<PacketCall::MessageType>(packetCall.getType()) == PacketCall::PUBLIC_CALL;

  if (!m_databaseService->accountExistById(netUserId) || !userData->hasContact(netUserId))
    {
      user->deliver(new PacketAnswer(PacketAnswer::CALL_USER_NOT_FOUND));
      return;
    }
  if (!netUserController)
    {
      user->deliver(new PacketAnswer(PacketAnswer::CALL_USER_OFFLINE));
      return;
    }

  UserData *netUserData = netUserController->getDataContainer().getData<UserData>("UserData");
  CallGroup &callGroup = getOrCreateCallGroup(userData->getId());
  PacketOnCall *packetOnCall = new PacketOnCall();

  if (isPublicCall)
    {
      if (!m_channelService->roomExistsById(roomId))
	{
	  user->deliver(new PacketAnswer(PacketAnswer::CALL_ROOM_DOESNT_EXIST));
	  return;
	}

      // TODO : catch throw of findRoomById
      const Room &room = m_channelService->findRoomById(roomId);
      if (room.isUserInRoom(callGroup.getHostUserId()))
	{
	  user->deliver(new PacketAnswer(PacketAnswer::CALL_HOST_NOT_IN_ROOM));
	  return;
	}
      if (room.isUserInRoom(netUserId))
	{
	  user->deliver(new PacketAnswer(PacketAnswer::CALL_USER_NOT_IN_ROOM));
	  return;
	}
      UserController *hostUserController = m_userService->findConnectedUserById(callGroup.getHostUserId());
      if (hostUserController){
	packetOnCall->setHostIp(hostUserController->getIpAdress());
      }
    }
  else
    {
      packetOnCall->setHostIp(user->getIpAdress());
    }

  callGroup.addUserInCallGroup(userData->getId());

  packetOnCall->setUserId(userData->getId());
  packetOnCall->setPort(packetCall.getPort());
  packetOnCall->setType(packetCall.getType());
  packetOnCall->setRoomId(packetCall.getRoomId());
  netUserController->deliver(packetOnCall);

  user->deliver(new PacketAnswer(PacketAnswer::CALL_REQUEST_SENT));
}

void		CallService::callJoin(SerializerBuffer *buffer, UserController *user)
{
  PacketCallJoin	packetCallJoin;

  try
    {
      packetCallJoin.unserialize(buffer);
    }
  catch (const PacketException &e)
    {
      user->deliver(new PacketAnswer(e.getError()));
      std::cout << "[CallService::callJoin] Warning : " << e.what() << std::endl;
      return ;
    }

  if (callGroupExistsByHostUser(packetCallJoin.getUserId()))
    {
      UserData *userData = user->getDataContainer().getData<UserData>("UserData");

      if (!userData->hasContact(packetCallJoin.getUserId()))
	{
	  // if the rfc add a answer code for 'call join not friend' add it here.
	  return ;
	}
      UserController *hostUserController = m_userService->findConnectedUserById(packetCallJoin.getUserId());
      if (hostUserController)
	{
	  PacketCallOnJoin *packetCallOnJoin = new PacketCallOnJoin;
	  packetCallOnJoin->setClientPort(packetCallJoin.getPort());
	  packetCallOnJoin->setUserId(userData->getId());
	  packetCallOnJoin->setClientIp(user->getIpAdress());

	  hostUserController->deliver(packetCallOnJoin);

	  CallGroup &callGroup = getOrCreateCallGroup(packetCallJoin.getUserId());
	  callGroup.addUserInCallGroup(userData->getId());
	}
      return ;
      // TODO : if the rfc add a answer code for 'hostUser not found in channel' add it here
    }
}

void		CallService::callQuit(SerializerBuffer *buffer, UserController *user)
{
  PacketCallQuit	packetCallQuit;

  try
    {
      packetCallQuit.unserialize(buffer);
    }
  catch (const PacketException &e)
    {
      user->deliver(new PacketAnswer(e.getError()));
      std::cout << "[CallService::callQuit] Warning : " << e.what() << std::endl;
      return ;
    }
  UserData *userData = user->getDataContainer().getData<UserData>("UserData");
  UserController *hostUserController = m_userService->findConnectedUserById(packetCallQuit.getUserId());
  if (!hostUserController)
    return ;
  UserData *hostUserData = hostUserController->getDataContainer().getData<UserData>("UserData");
  if (!callGroupExistsByHostUser(hostUserData->getId()))
    return ;

  CallGroup &callGroup = getOrCreateCallGroup(hostUserData->getId());
  const std::vector<uint32_t> &usersInCallGroup = callGroup.getUsers();
  callGroup.removeUserInCallGroup(userData->getId());

  if (!usersInCallGroup.size())
    {
      std::cout << "[CallService::callQuit] CallGroup removed" << std::endl;
      removeCallGroup(callGroup.getHostUserId());
      return ;
    }

  for (std::vector<uint32_t>::const_iterator it = usersInCallGroup.begin();
       it != usersInCallGroup.end();
       ++it)
    {
      UserController *userInCallGroup = m_userService->findConnectedUserById(*it);
      if (userInCallGroup)
	{
	  if (hostUserData->getId() == userData->getId())
	    {
	      PacketCallStop *packetCallStop = new PacketCallStop;
	      packetCallStop->setUserId(userData->getId());
	      userInCallGroup->deliver(packetCallStop);
	    }
	  else
	    {
	      PacketCallOnQuit *packetCallOnQuit = new PacketCallOnQuit;
	      packetCallOnQuit->setUserId(userData->getId());
	      userInCallGroup->deliver(packetCallOnQuit);
	    }
	}
    }
}
