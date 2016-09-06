#include	<iostream>
#include	"UserService.hh"
#include	"../../UserController.hh"
#include	"../../services/UserData.hh"
#include	"../../packets/PacketAnswer.hh"
#include	"../../packets/PacketUpdateStatus.hh"
#include	"../../packets/PacketUpdateUsername.hh"
#include	"../../packets/PacketAddContact.hh"
#include	"../../packets/PacketLoginSuccess.hh"
#include	"../../packets/PacketSearchUser.hh"
#include	"../../packets/PacketSearchResult.hh"
#include	"../../packets/PacketException.hh"
#include	"../../packets/PacketContactRequest.hh"
#include	"../../packets/PacketContactList.hh"
#include	"../../packets/PacketContactAdded.hh"
#include	"../../packets/PacketRefuseContact.hh"
#include	"../../packets/PacketContactRefused.hh"
#include	"../../packets/PacketUserEvent.hh"
#include	"../../ConfManager.hh"

const std::string UserService::m_userServiceName = std::string("UserService");

UserService::UserService()
{
  m_packetIds.push_front(APacket::ID_ADD_CONTACT);
  m_packetIds.push_front(APacket::ID_REFUSE_CONTACT);
  m_packetIds.push_front(APacket::ID_SEARCH_USER);
  m_packetIds.push_front(APacket::ID_UPDATE_USERNAME);
  m_packetIds.push_front(APacket::ID_UPDATE_STATUS);

  m_services[APacket::ID_ADD_CONTACT] = &UserService::addContact;
  m_services[APacket::ID_REFUSE_CONTACT] = &UserService::refuseContact;
  m_services[APacket::ID_SEARCH_USER] = &UserService::searchUser;
  m_services[APacket::ID_UPDATE_USERNAME] = &UserService::updateUserName;
  m_services[APacket::ID_UPDATE_STATUS] = &UserService::updateStatus;
}

bool		UserService::startService(ServiceManager *serviceManager)
{
  IService *databaseService = serviceManager->getService("DatabaseService");
  if (!databaseService || !(m_database = dynamic_cast<IDatabase *>(databaseService)))
    {
      std::cout << "[" << m_userServiceName << "]  : "
		<< "Warning, DatabaseService not found." << std::endl;
      return false;
    }

  return true;
}

std::string	UserService::getServiceName() const
{
  return UserService::m_userServiceName;
}

void UserService::stopService()
{

}

const std::list<APacket::packetID>	&UserService::getPacketIds() const
{
  return m_packetIds;
}

void		UserService::handlePacket(APacket::packetID packetId, SerializerBuffer *buffer,
					  UserController *user)
{
  std::cout << "[UserService] : executeTask..." << std::endl;
  if (!user->isAuthentificated())
    {
      std::cout << "[UserService] : warning an user is not authenticate and he try to communicate with unauthorized service..." << std::endl;
      user->disconnect();
      return ;
    }
  void (UserService::*service)(SerializerBuffer *buffer, UserController *user);

  if ((service = m_services[packetId]))
    (this->*service)(buffer, user);
}

void UserService::onServerEventClientDisconnected(UserController *user)
{
  for (std::list<UserController *>::iterator it = m_connectedUsers.begin();
       it != m_connectedUsers.end();
       ++it)
    {
      if (*it == user)
	{
	  UserData *userData = (*it)->getDataContainer().getData<UserData>("UserData");
	  std::cout << "[UserService::onServerEventClientDisconnected] user '"
		    << userData->getUsername() << "' removed from "
		    << getServiceName() << std::endl;
	  m_connectedUsers.erase(it);
	  return ;
	}
    }
}

void UserService::onServerEventClientConnected(UserController *user)
{}

/**
 * UserService::isUserConnected
 *
 * Return true if the user 'userID' exists and is connected
 */
bool		UserService::isUserConnected(uint32_t userID) const
{
  UserController const * const user = findConnectedUserById(userID);

  if (user)
    return user->isAuthentificated();
  return false;
}

UserController	*UserService::findConnectedUserById(uint32_t idToFind) const
{
  for (std::list<UserController *>::const_iterator it = m_connectedUsers.begin();
       it != m_connectedUsers.end();
       ++it)
    {
      UserData *userData = (*it)->getDataContainer().getData<UserData>("UserData");
      if (userData->getId() == idToFind)
	return *it;
    }
  return NULL;
}

std::list<UserController *>	UserService::findConnectedUserByUsername(const std::string &usernameToFind) const
{
  std::list<UserController *> userControllerList;

  for (std::list<UserController *>::const_iterator it = m_connectedUsers.begin();
       it != m_connectedUsers.end();
       ++it)
    {
      UserData *userData = (*it)->getDataContainer().getData<UserData>("UserData");
      if (userData->getUsername() == usernameToFind)
	userControllerList.push_front(*it);
    }
  return userControllerList;
}

/**
 * When a user log in then this method is called.
 *
 * It send in order the packets :
 *
 * loggin success
 * contact list
 * contact request list
 *
 */

void	UserService::addConnectedUser(UserController *userToAdd)
{
  UserController *contactUserController;
  UserData *contactUserData;
  PacketContactList *packetContactList = new PacketContactList;
  UserData *userData = userToAdd->getDataContainer().getData<UserData>("UserData");

  UserController *alreadyConnectedUser = findConnectedUserById(userData->getId());
  if (alreadyConnectedUser)
    {
      alreadyConnectedUser->disconnect();
      delConnectedUser(alreadyConnectedUser);
    }

  PacketLoginSuccess *packetLoginSuccess = new PacketLoginSuccess;
  packetLoginSuccess->setUserData(userData->getId(), userData->getUsername(), userData->getStatus());
  userToAdd->deliver(packetLoginSuccess);

  const std::vector<uint32_t> &contacts = userData->getContacts();
  std::cout << "[UserService::addConnectedUser] " << userData->getUsername() << " has "
	    << contacts.size() << " contacts." << std::endl;
  for (std::vector<uint32_t>::const_iterator it = contacts.begin();
       it != contacts.end();
       ++it)
    {
      UserController *contactUserController = findConnectedUserById(*it);
      UserData offlineContactUserData;
      UserData *contactUserData;
      if (!contactUserController)
	{
	  try {
	    offlineContactUserData = m_database->getAccountById(*it);
	  } catch (const DatabaseException &e) {
	    continue;
	  }
	  contactUserData = &offlineContactUserData;
	}
      else
	{
	  contactUserData = contactUserController->getDataContainer().getData<UserData>("UserData");
	}
      packetContactList->addContact(contactUserData->getId(),
				    contactUserData->getUsername(),
				    contactUserData->getStatus());
    }
  userToAdd->deliver(packetContactList);

  const std::vector<uint32_t> &contactsRequest = userData->getContactsRequest();
  std::cout << "[UserService::addConnectedUser] " << userData->getUsername() << " has "
	    << contactsRequest.size() << " pending requests." << std::endl;
  for (std::vector<uint32_t>::const_iterator it = contactsRequest.begin();
       it != contactsRequest.end();
       ++it)
    {
      UserController *contactUserController = findConnectedUserById(*it);
      UserData offlineContactUserData;
      UserData *contactUserData;
      if (!contactUserController)
	{
	  try {
	    offlineContactUserData = m_database->getAccountById(*it);
	  } catch (const DatabaseException &e) {
	    // TODO : the id of the contact that send a contact request doesnt exists.
	    continue;
	  }
	  contactUserData = &offlineContactUserData;
	}
      else
	{
	  contactUserData = contactUserController->getDataContainer().getData<UserData>("UserData");
	}
      PacketContactRequest *packetContactRequest = new PacketContactRequest;
      packetContactRequest->setUserData(contactUserData->getId(),
					contactUserData->getUsername(),
					contactUserData->getStatus());
      userToAdd->deliver(packetContactRequest);
    }

  m_connectedUsers.push_front(userToAdd);
}

bool	UserService::delConnectedUser(UserController *userToDel)
{
  for (std::list<UserController *>::iterator it = m_connectedUsers.begin();
       it != m_connectedUsers.end();
       ++it)
    {
      if ((*it) == userToDel)
	{
	  m_connectedUsers.erase(it);
	  return true;
	}
    }
  return false;
}

size_t	UserService::countConnectedUsers() const
{
  return m_connectedUsers.size();
}

/**
 * Private methods : packets handler
 *
 * In order :
 *
 * UserService::searchUser
 * UserService::updateUserName
 * UserService::updateStatus
 * UserService::addContact
 *
 */

void		UserService::searchUser(SerializerBuffer *buffer, UserController *user)
{
  PacketSearchUser	paketSearchUser;
  try{
	  paketSearchUser.unserialize(buffer);
  } catch (const PacketException &e)
    {
      user->deliver(new PacketAnswer(e.getError()));
      std::cout << "[UserService] Warning : " << e.what() << std::endl;
      return ;
    }

  std::string		pattern = paketSearchUser.getPattern();
  PacketSearchResult	*packetSearchResult = new PacketSearchResult;
  bool			found = false;

  const std::vector<UserData> userSearch = m_database->searchUsersByUsername(pattern);

		std::cout << "userSearchResult : " << std::endl;
  for (std::vector<UserData>::const_iterator it = userSearch.begin();
       it != userSearch.end();
       ++it)
    {
		std::cout << "- " << it->getUsername() << ", " << it->getStatus() << std::endl;
      packetSearchResult->addUserInlist(it->getId(), it->getUsername(), it->getStatus());
      found = true;
    }

  if (!found)
    {
      delete packetSearchResult;
      std::cout << "[UserService::searchUser] : User not found." << std::endl;
      user->deliver(new PacketAnswer(PacketAnswer::SEARCH_USER_NOT_FOUND));
      return ;
    }
  else
    user->deliver(packetSearchResult);
}

void		UserService::updateUserName(SerializerBuffer *buffer, UserController *user)
{
  PacketUpdateUsername	updateUsernamePacket;

  try
    {
      updateUsernamePacket.unserialize(buffer);
    }
  catch (const PacketException &e)
    {
      user->deliver(new PacketAnswer(e.getError()));
      std::cout << "[UserService] Warning : " << e.what() << std::endl;
      return ;
    }

  UserData *userData = user->getDataContainer().getData<UserData>("UserData");
  userData->setUsername(updateUsernamePacket.getUsername());
  try{
  m_database->updateAccount(userData->getId(), *userData);
      } catch (const DatabaseException &e) {
	std::cerr << e.what() << std::endl;
      }
  user->deliver(new PacketAnswer(PacketAnswer::UPDATE_USERNAME_SUCCESS));


  UserController *contactUserController;
  const std::vector<uint32_t> contacts = userData->getContacts();
  for (std::vector<uint32_t>::const_iterator it = contacts.begin();
       it != contacts.end();
       ++it)
    {
      if ((contactUserController = findConnectedUserById(*it)) != NULL)
	{
	  PacketUserEvent *packetUserEvent = new PacketUserEvent;
	  packetUserEvent->setUserId(userData->getId());
	  packetUserEvent->setIdEvent(PacketUserEvent::NICKNAME_CHANGED);
	  packetUserEvent->setData(userData->getUsername());
	  contactUserController->deliver(packetUserEvent);
	}
    }

  PacketUserEvent *packetUserEvent = new PacketUserEvent;
  packetUserEvent->setUserId(userData->getId());
  packetUserEvent->setIdEvent(PacketUserEvent::NICKNAME_CHANGED);
  packetUserEvent->setData(userData->getUsername());
  user->deliver(packetUserEvent);

}

void		UserService::updateStatus(SerializerBuffer *buffer, UserController *user)
{
  PacketUpdateStatus	updateStatusPacket;

  try
    {
      updateStatusPacket.unserialize(buffer);
    }
  catch (const PacketException &e)
    {
      user->deliver(new PacketAnswer(e.getError()));
      std::cout << "[UserService] Warning : " << e.what() << std::endl;
      return ;
    }

  UserData *userData = user->getDataContainer().getData<UserData>("UserData");
  userData->setStatus(updateStatusPacket.getStatus());
  try{
  m_database->updateAccount(userData->getId(), *userData);
      } catch (const DatabaseException &e) {
	std::cerr << e.what() << std::endl;
      }
  user->deliver(new PacketAnswer(PacketAnswer::UPDATE_STATUS_SUCCESS));

  UserController *contactUserController;
  const std::vector<uint32_t> contacts = userData->getContacts();
  for (std::vector<uint32_t>::const_iterator it = contacts.begin();
       it != contacts.end();
       ++it)
    {
      if ((contactUserController = findConnectedUserById(*it)) != NULL)
	{
	  PacketUserEvent *packetUserEvent = new PacketUserEvent;
	  packetUserEvent->setUserId(userData->getId());
	  packetUserEvent->setIdEvent(PacketUserEvent::STATUS_CHANGED);
	  packetUserEvent->setData(userData->getStatus());
	  contactUserController->deliver(packetUserEvent);
	}
    }

  PacketUserEvent *packetUserEvent = new PacketUserEvent;
  packetUserEvent->setUserId(userData->getId());
  packetUserEvent->setIdEvent(PacketUserEvent::STATUS_CHANGED);
  packetUserEvent->setData(userData->getStatus());
  user->deliver(packetUserEvent);
}

void		UserService::refuseContact(SerializerBuffer *buffer, UserController *user)
{
  PacketRefuseContact packetRefuseContact;
  packetRefuseContact.unserialize(buffer);


  UserData *userData = user->getDataContainer().getData<UserData>("UserData");
  userData->removeContactRequest(packetRefuseContact.getUserId());
  try{
  m_database->updateAccount(userData->getId(), *userData);
      } catch (const DatabaseException &e) {
	std::cerr << e.what() << std::endl;
      }

  UserController *controller = findConnectedUserById(packetRefuseContact.getUserId());
  bool isContactOnline = true;
  UserData offlineContactUserData;
  UserData *contactUserData;
  if (!controller)
    {
      try {
	offlineContactUserData = m_database->getAccountById(packetRefuseContact.getUserId());
      } catch (const DatabaseException &e) {
	std::cerr << e.what() << std::endl;
	return ;
      }
      contactUserData = &offlineContactUserData;
      isContactOnline = false;
    }
  else
    {
      contactUserData = controller->getDataContainer().getData<UserData>("UserData");
    }
  if (isContactOnline)
    {
      PacketContactRefused *packetContactRefused = new PacketContactRefused();
      packetContactRefused->setUserData(userData->getId(), userData->getUsername(), userData->getStatus());
      controller->deliver(packetContactRefused);
    }
}

void		UserService::addContact(SerializerBuffer *buffer, UserController *user)
{
  PacketAddContact	addContactPacket;

  addContactPacket.unserialize(buffer);

  UserData *userData = user->getDataContainer().getData<UserData>("UserData");
  if (userData->getId() == addContactPacket.getUserId())
  {
      user->deliver(new PacketAnswer(PacketAnswer::ADD_CONTACT_ALREADY_FRIEND));
	  return;
  }
  UserController *contactToAdd = findConnectedUserById(addContactPacket.getUserId());

  bool isContactOnline = true;
  UserData offlineContactUserData;
  UserData *contactUserData;
  if (!contactToAdd)
    {
      try {
	offlineContactUserData = m_database->getAccountById(addContactPacket.getUserId());
      } catch (const DatabaseException &e) {
	user->deliver(new PacketAnswer(PacketAnswer::ADD_CONTACT_USER_ID_NOT_FOUND));
	return ;
      }
      contactUserData = &offlineContactUserData;
      isContactOnline = false;
    }
  else
    {
      contactUserData = contactToAdd->getDataContainer().getData<UserData>("UserData");
    }


  bool isInUserFriend = userData->hasContact(contactUserData->getId());
  bool isInUserRequestFriend = userData->hasContactRequest(contactUserData->getId());

  bool isInContactFriend = contactUserData->hasContact(userData->getId());
  bool isInContactRequestFriend = contactUserData->hasContactRequest(userData->getId());

  // TODO : Need to be changed when the rfc will add a answer for 'client try to add himself as a contact'
  if ((isInUserFriend && isInContactFriend) || (userData->getId() == contactUserData->getId()))
    {
      user->deliver(new PacketAnswer(PacketAnswer::ADD_CONTACT_ALREADY_FRIEND));
      return ;
    }

  if (isInContactRequestFriend)
    {
      user->deliver(new PacketAnswer(PacketAnswer::ADD_CONTACT_REQUEST_ALREADY_SEND));
      return ;
    }
  else if (isInUserRequestFriend)
    {
      user->deliver(new PacketAnswer(PacketAnswer::ADD_CONTACT_SUCCESS_FRIEND));
      if (isContactOnline)
		{
		  PacketContactAdded *packetAdded = new PacketContactAdded();
		  packetAdded->setUserData(userData->getId(), userData->getUsername(), userData->getStatus());
		  contactToAdd->deliver(packetAdded);
		}
      PacketContactAdded *packetAdded = new PacketContactAdded();
      packetAdded->setUserData(contactUserData->getId(),
			       contactUserData->getUsername(),
			       contactUserData->getStatus());
      user->deliver(packetAdded);

      contactUserData->addContact(userData->getId());
      userData->addContact(contactUserData->getId());
      userData->removeContactRequest(contactUserData->getId());

  try{
      m_database->updateAccount(userData->getId(), *userData);
      m_database->updateAccount(contactUserData->getId(), *contactUserData);
      } catch (const DatabaseException &e) {
	std::cerr << e.what() << std::endl;
      }
      return ;
    }

  user->deliver(new PacketAnswer(PacketAnswer::ADD_CONTACT_SUCCESS_WAITING));

  if (isContactOnline)
    {
       PacketContactRequest *packetContactRequest = new PacketContactRequest;
       packetContactRequest->setUserData(userData->getId(), userData->getUsername(), userData->getStatus());
       contactToAdd->deliver(packetContactRequest);
    }
  contactUserData->addContactRequest(userData->getId());
  try{
    m_database->updateAccount(contactUserData->getId(), *contactUserData);
  } catch (const DatabaseException &e) {
    std::cerr << e.what() << std::endl;
  }
}
