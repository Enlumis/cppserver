#include	<iostream>
#include	"ChannelService.hh"
#include	"../../UserController.hh"
#include	"../../packets/PacketAnswer.hh"
#include	"../../packets/PacketException.hh"
#include	"../../services/UserData.hh"
#include	"../../packets/PacketCreateChannel.hh"
#include	"../../packets/PacketOpenChannel.hh"
#include	"../../packets/PacketAddUserToChannel.hh"
#include	"../../packets/PacketSendMessage.hh"
#include	"../../packets/PacketRecvMessage.hh"
#include	"../../packets/PacketChannelEvent.hh"
#include	"../../packets/PacketCall.hh"
#include	"../../packets/PacketOnCall.hh"
#include	"../../packets/PacketCallJoin.hh"
#include	"../../packets/PacketCallOnJoin.hh"
#include	"../../packets/PacketCallOnQuit.hh"
#include	"../../packets/PacketCallQuit.hh"

ChannelService::ChannelService()
  : m_currentMaxChannelId(1)
{
  m_packetIds.push_front(APacket::ID_CREATE_CHANNEL);
  m_packetIds.push_front(APacket::ID_ADD_USER_TO_CHANNEL);
  m_packetIds.push_front(APacket::ID_SEND_MESSAGE);

  m_services[APacket::ID_CREATE_CHANNEL] = &ChannelService::createChannel;
  m_services[APacket::ID_ADD_USER_TO_CHANNEL] = &ChannelService::addUserToChannel;
  m_services[APacket::ID_SEND_MESSAGE] = &ChannelService::sendMessage;
}

ChannelService::~ChannelService()
{

}

void ChannelService::onServerEventClientDisconnected(UserController *user)
{
  UserData *userData = user->getDataContainer().getData<UserData>("UserData");
  std::map<uint32_t, Room>::iterator room = this->m_channels.begin();

  while (room != this->m_channels.end())
    {
      if (room->second.isUserInRoom(userData->getId()))
	{

	  room->second.removeUserInRoom(userData->getId());

	  for (std::list<uint32_t>::const_iterator userInRoom = room->second.getUsers().begin();
	       userInRoom != room->second.getUsers().end();
	       ++userInRoom)
	    {
	      UserController *userController;
	      if ((userController = m_userService->findConnectedUserById(*userInRoom)) != NULL)
		{
		  PacketChannelEvent *packetChannelEvent = new PacketChannelEvent;
		  UserData *data = userController->getDataContainer().getData<UserData>("UserData");
		  packetChannelEvent->setIdEvent(PacketChannelEvent::USER_LOGGOUT);
		  std::cout << "send quit id:" << userData->getId() << ": " << userData->getUsername() << "  to  "
			    << data->getId() << ":" << data->getUsername() << std::endl;
		  packetChannelEvent->setChannelId(room->second.getRoomId());
		  packetChannelEvent->setUserId(userData->getId());
		  packetChannelEvent->setUsername(userData->getUsername());
		  packetChannelEvent->setStatus(userData->getStatus());

		  userController->deliver(packetChannelEvent);
		}
	    }
	  if (room->second.getUsers().size() == 1){
	    room = this->m_channels.erase(room);
	  }else{
	    ++room;
	  }
	}else{
	++room;
      }
    }

}
void ChannelService::onServerEventClientConnected(UserController *user)
{
}

void	ChannelService::handlePacket(APacket::packetID packetid, SerializerBuffer *buffer, UserController *user)
{
  std::cout << "[ ChannelService ] : executeTask..." << std::endl;
  if (!user->isAuthentificated())
    {
      std::cout << "[ ChannelService ] : warning an user is not authenticate and he try to communicate with unauthorized service..." << std::endl;
      user->disconnect();
    }
  void (ChannelService::*service)(SerializerBuffer *buffer, UserController *user);

  if ((service = m_services[packetid]))
    (this->*service)(buffer, user);
}

bool	ChannelService::	startService(ServiceManager *serviceManager)
{
  IService *databaseService = serviceManager->getService("DatabaseService");
  if (!databaseService || !(m_databaseService = dynamic_cast<IDatabase *>(databaseService)))
    {
      std::cout << "[ " << this->getServiceName() << " ]  : "
		<< "Warning, DatabaseService not found." << std::endl;
      return false;
    }

  IService *userService = serviceManager->getService("UserService");
  if (!userService || !(m_userService = dynamic_cast<IUserService *>(userService)))
    {
      std::cout << "[ " << getServiceName() << " ]  : "
		<< "Warning, UserService not found." << std::endl;
      return false;
    }

  return true;
}

void	ChannelService::	stopService()
{

}

std::string	ChannelService::getServiceName() const
{
  return "ChannelService";
}

const std::list<APacket::packetID>	&ChannelService::getPacketIds() const
{
  return m_packetIds;
}

const Room	&ChannelService::findRoomById(uint32_t roomId) const
{
  std::map<uint32_t, Room>::const_iterator it = m_channels.find(roomId);
  if (it == m_channels.end())
    {
      // TODO : throw somthing ?
    }
  return it->second;
}

const bool	ChannelService::roomExistsById(uint32_t roomId) const
{
  std::map<uint32_t, Room>::const_iterator it = m_channels.find(roomId);
  if (it == m_channels.end())
    return false;
  return true;
}

const std::map<uint32_t, Room>	&ChannelService::getRooms() const
{
  return m_channels;
}

/**
 * Packets handler.
 *
 * In order :
 *
 * createChannel
 * addUserToChannel
 * sendMessage
 *
 */

/**
 * createChannel
 *
 * In order :
 *
 * -> check if users exists and are connected
 * -> check if a channel exists with exactly the sames users inside
 * -> create the channel
 * -> send the open_channel packets
 */
void		ChannelService::createChannel(SerializerBuffer *buffer, UserController *user)
{
  PacketCreateChannel	packetCreateChannel;

  try
    {
      packetCreateChannel.unserialize(buffer);
    }
  catch (const PacketException &e)
    {
      user->deliver(new PacketAnswer(e.getError()));
      std::cout << "[ChannelService::createChannel] Warning : " << e.what() << std::endl;
      return ;
    }

  const std::list<uint32_t> usersInNewChannel = packetCreateChannel.getUserIds();

  for (std::list<uint32_t>::const_iterator itNewUsers = usersInNewChannel.begin();
       itNewUsers != usersInNewChannel.end();
       ++itNewUsers)
    {
      if (!m_databaseService->accountExistById(*itNewUsers))
	{
	  user->deliver(new PacketAnswer(PacketAnswer::CREATE_CHANNEL_USERS_DOESNT_EXISTS));
	  return ;
	}
      if (!m_userService->isUserConnected(*itNewUsers))
	{
	  user->deliver(new PacketAnswer(PacketAnswer::CREATE_CHANNEL_USERS_NOT_CONNECTED));
	  return ;
	}
    }

  bool found = false;
  int foundCount = 0;
  for (std::map<uint32_t, Room>::const_iterator it = m_channels.begin();
       it != m_channels.end();
       ++it)
    {
      const std::list<uint32_t> usersInChannel = it->second.getUsers();
      if (usersInChannel.size() == usersInNewChannel.size())
	{
	  for (std::list<uint32_t>::const_iterator it = usersInChannel.begin();
	       it != usersInChannel.end();
	       ++it)
	    {
	      found = false;
	      foundCount = 0;
	      for (std::list<uint32_t>::const_iterator itNewUsers = usersInNewChannel.begin();
		   itNewUsers != usersInNewChannel.end();
		   ++itNewUsers)
		{
		  if (*itNewUsers == *it)
		    {
		      foundCount++;
		    }
		}
	      if (foundCount == usersInNewChannel.size())
		{
		  found = true;
		  break;
		}
	      if (!found)
		break;
	    }
	}

      if (found)
	{
	  user->deliver(new PacketAnswer(PacketAnswer::CREATE_CHANNEL_ALREADY_EXISTS));
	  return ;
	}
    }

  Room newChannel("new Channel", m_currentMaxChannelId, usersInNewChannel);
  //newChannel.setHostUserId(user->getDataContainer().getData<UserData>("UserData")->getId());
  m_channels.insert(std::make_pair(m_currentMaxChannelId, newChannel));


  for (std::list<uint32_t>::const_iterator itNewUsers = usersInNewChannel.begin();
       itNewUsers != usersInNewChannel.end();
       ++itNewUsers)
    {
      PacketOpenChannel *packectOpenChannel = new PacketOpenChannel;
      UserController *userController = m_userService->findConnectedUserById(*itNewUsers);
      for (std::list<uint32_t>::const_iterator itNewUsers2 = usersInNewChannel.begin();
	   itNewUsers2 != usersInNewChannel.end();
	   ++itNewUsers2)
	{
	  UserController *userController2 = m_userService->findConnectedUserById(*itNewUsers2);
	  if (userController2)
	    {
	      UserData *userData2 = userController2->getDataContainer().getData<UserData>("UserData");
	      packectOpenChannel->setUserdata(userData2->getId(), userData2->getUsername(), userData2->getStatus());
	    }
	}
      if (userController)
	{
	  packectOpenChannel->setChannelId(m_currentMaxChannelId);
	  userController->deliver(packectOpenChannel);
	}
    }

  ++m_currentMaxChannelId;
}

void		ChannelService::addUserToChannel(SerializerBuffer *buffer, UserController *user)
{
  PacketAddUserToChannel	packetAddUserToChannel;

  packetAddUserToChannel.unserialize(buffer);
  std::cout << "addUserToCHannel (" << packetAddUserToChannel.getChannelId() << " )" << std::endl;
  uint32_t newUserID = packetAddUserToChannel.getNewUserId();
  UserController *newUserController;
  if ((newUserController = m_userService->findConnectedUserById(newUserID)) == NULL)
    {
      user->deliver(new PacketAnswer(PacketAnswer::ADD_USER_TO_CHANNEL_USER_DOESNT_EXISTS));
      return ;
    }

  uint32_t channelId = packetAddUserToChannel.getChannelId();
  std::map<uint32_t, Room>::iterator chanIt = m_channels.find(channelId);
  if (chanIt == m_channels.end())
    {
      user->deliver(new PacketAnswer(PacketAnswer::ADD_USER_TO_CHANNEL_CHANID_DOESNT_EXISTS));
      return;
    }

  Room channel = chanIt->second;
  UserData *userData = user->getDataContainer().getData<UserData>("UserData");
  if (!channel.isUserInRoom(userData->getId()) || channel.isUserInRoom(newUserID))
    {
      user->deliver(new PacketAnswer(PacketAnswer::ADD_USER_TO_CHANNEL_PERMISSION_DENIED));
      return;
    }

  if (!userData->hasContact(newUserID))
    {
      user->deliver(new PacketAnswer(PacketAnswer::ADD_USER_TO_CHANNEL_USER_NOT_IN_CONTACT_LIST));
      return;
    }

  std::cout << "addUserToCHannel SUCCESS" << std::endl;
  user->deliver(new PacketAnswer(PacketAnswer::ADD_USER_TO_CHANNEL_SUCCESS));
  UserController *userController;
  UserData *newUserData = newUserController->getDataContainer().getData<UserData>("UserData");

  const std::list<uint32_t> roomUsers = channel.getUsers();
  for (std::list<uint32_t>::const_iterator it = roomUsers.begin();
       it != roomUsers.end();
       ++it)
    {
      if ((userController = m_userService->findConnectedUserById(*it)) != NULL)
	{
	  PacketChannelEvent *packetChannelEvent = new PacketChannelEvent;
	  packetChannelEvent->setIdEvent(PacketChannelEvent::USER_LOGGIN);
	  packetChannelEvent->setChannelId(channel.getRoomId());
	  packetChannelEvent->setUserId(newUserID);
	  packetChannelEvent->setUsername(newUserData->getUsername());
	  packetChannelEvent->setStatus(newUserData->getStatus());

	  userController->deliver(packetChannelEvent);
	}
    }
  chanIt->second.addUserInRoom(newUserID);


  PacketOpenChannel *packectOpenChannel = new PacketOpenChannel;
  for (std::list<uint32_t>::const_iterator itNewUsers2 = chanIt->second.getUsers().begin();
       itNewUsers2 != chanIt->second.getUsers().end();
       ++itNewUsers2)
    {
      UserController *userController2 = m_userService->findConnectedUserById(*itNewUsers2);
      if (userController2)
	{
	  UserData *userData2 = userController2->getDataContainer().getData<UserData>("UserData");
	  std::cout << " - " << userData2->getId() << " : " << userData2->getUsername() << std::endl;
	  packectOpenChannel->setUserdata(userData2->getId(), userData2->getUsername(), userData2->getStatus());
	}
    }
  packectOpenChannel->setChannelId(channelId);
  newUserController->deliver(packectOpenChannel);
}

void		ChannelService::sendMessage(SerializerBuffer *buffer, UserController *user)
{
  PacketSendMessage	packetSendMessage;

  try
    {
      packetSendMessage.unserialize(buffer);
    }
  catch (const PacketException &e)
    {
      user->deliver(new PacketAnswer(e.getError()));
      std::cout << "[ChannelService::sendMessage] Warning : " << e.what() << std::endl;
      return ;
    }

  UserData *userData = user->getDataContainer().getData<UserData>("UserData");
  UserController *userController;
  if (packetSendMessage.getType() == PacketSendMessage::USER_MESSAGE)
    {
      uint32_t destUserId = packetSendMessage.getChanOrUserId();
      if ((userController = m_userService->findConnectedUserById(destUserId)) != NULL)
	{
	  UserData *contactUserData = userController->getDataContainer().getData<UserData>("UserData");
	  if (!userData->hasContact(contactUserData->getId()))
	    {
	      user->deliver(new PacketAnswer(PacketAnswer::SEND_MESSAGE_PERMISSION_DENIED));
	      return ;
	    }
	  PacketRecvMessage *packetRecvMessage = new PacketRecvMessage;
	  packetRecvMessage->setType(PacketRecvMessage::USER_MESSAGE);
	  packetRecvMessage->setChanOrUserId(destUserId);
	  packetRecvMessage->setSenderId(userData->getId());
	  packetRecvMessage->setMessage(packetSendMessage.getMessage());

	  user->deliver(packetRecvMessage);


	  packetRecvMessage = new PacketRecvMessage;

	  packetRecvMessage->setType(PacketRecvMessage::USER_MESSAGE);
	  packetRecvMessage->setChanOrUserId(destUserId);
	  packetRecvMessage->setSenderId(userData->getId());
	  packetRecvMessage->setMessage(packetSendMessage.getMessage());

	  userController->deliver(packetRecvMessage);
	  return ;
	}
      user->deliver(new PacketAnswer(PacketAnswer::SEND_MESSAGE_CHAN_USER_DOESNT_EXISTS));
    }
  else if (packetSendMessage.getType() == PacketSendMessage::CHANNEL_MESSAGE)
    {
      uint32_t channelId = packetSendMessage.getChanOrUserId();
      std::map<uint32_t, Room>::const_iterator chanIt = m_channels.find(channelId);
      if (chanIt == m_channels.end())
	{
	  user->deliver(new PacketAnswer(PacketAnswer::SEND_MESSAGE_CHAN_USER_DOESNT_EXISTS));
	  return;
	}
      Room channel = chanIt->second;
      PacketRecvMessage *packetRecvMessage;
      const std::list<uint32_t> roomUsers = channel.getUsers();
      if (!channel.isUserInRoom(userData->getId()))
	{
	  user->deliver(new PacketAnswer(PacketAnswer::SEND_MESSAGE_PERMISSION_DENIED));
	  return ;
	}
      for (std::list<uint32_t>::const_iterator it = roomUsers.begin();
	   it != roomUsers.end();
	   ++it)
	{
	  if ((userController = m_userService->findConnectedUserById(*it)) != NULL)
	    {
	      packetRecvMessage = new PacketRecvMessage;

	      packetRecvMessage->setType(PacketRecvMessage::CHANNEL_MESSAGE);
	      packetRecvMessage->setChanOrUserId(channelId);
	      packetRecvMessage->setSenderId(userData->getId());
	      packetRecvMessage->setMessage(packetSendMessage.getMessage());

	      userController->deliver(packetRecvMessage);
	    }
	}
    }
  else
    user->disconnect();
}
