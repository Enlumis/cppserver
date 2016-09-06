#include	<iostream>
#include	<string>
#include	"LogginService.hh"
#include	"../../packets/PacketAnswer.hh"
#include	"../../packets/PacketAuthentification.hh"
#include	"../../packets/PacketRegister.hh"
#include	"../../packets/PacketHello.hh"
#include	"../../services/UserData.hh"
#include	"../../services/database-service/IDatabase.hh"
#include	"../../utils/md5.hh"
#include	"../../packets/PacketException.hh"

const std::string LogginService::m_logginServiceName = "LogginService";

LogginService::LogginService()
{
  m_packetIds.push_front(APacket::ID_AUTHENTIFICATION);
  m_packetIds.push_front(APacket::ID_REGISTER);

  m_services[APacket::ID_AUTHENTIFICATION] = &LogginService::authentificationHandler;
  m_services[APacket::ID_REGISTER] = &LogginService::registerHandler;
}

LogginService::~LogginService()
{

}

void	LogginService::handlePacket(APacket::packetID packetId, SerializerBuffer *buffer,
				    UserController *user)
{
  std::cout << "[LogginService] : executeTask..." << std::endl;
  void (LogginService::*service)(SerializerBuffer *, UserController *);

  if ((service = m_services[packetId]))
     (this->*service)(buffer, user);
}

bool	LogginService::	startService(ServiceManager *serviceManager)
{
  IService *userService = serviceManager->getService("UserService");
  if (!userService || !(m_userService = dynamic_cast<IUserService *>(userService)))
    {
      std::cout << "[" << m_logginServiceName << "]  : "
		<< "Warning, UserService not found." << std::endl;
		return false;
    }
  IService *databaseService = serviceManager->getService("DatabaseService");
  if (!databaseService || !(m_database = dynamic_cast<IDatabase *>(databaseService)))
    {
      std::cout << "[" << m_logginServiceName << "]  : "
		<< "Warning, DatabaseService not found." << std::endl;
		return false;
    }
  return true;
}

void	LogginService::	stopService()
{

}

std::string	LogginService::getServiceName() const
{
  return LogginService::m_logginServiceName;
}

const std::list<APacket::packetID>	&LogginService::getPacketIds() const
{
  return m_packetIds;
}
void LogginService::onServerEventClientDisconnected(UserController *user)
{}

void LogginService::onServerEventClientConnected(UserController *user)
{
  PacketHello *helloPacket = new PacketHello;
  char *salt = new char[33];
  memcpy(salt, helloPacket->getSalt().c_str(), 32);
  salt[32] = '\0';
  user->getDataContainer().setData("SaltKey", salt);
  user->deliver(helloPacket);
}

void		LogginService::authentificationHandler(SerializerBuffer *buffer, UserController *user)
{
  PacketAuthentification	authPacket;

  try
    {
      authPacket.unserialize(buffer);
    }
  catch (const PacketException &e)
    {
      user->deliver(new PacketAnswer(e.getError()));
      std::cout << "[LogginService::authentificationHandler] Warning : " << e.what() << std::endl;
      return ;
    }

  std::string accountName = authPacket.getAccountName();
  if (!m_database->accountExistByName(accountName))
    {
      std::cout << "[LogginService::authentificationHandler] account name '"
		<< accountName << "' doesn't exists." << std::endl;
      user->deliver(new PacketAnswer(PacketAnswer::AUTH_ACCOUNT_NAME_NOT_FOUND));
      return;
    }

  UserData userData;
  try
    {
 userData = m_database->getAccountByName(accountName);
      } catch (const DatabaseException &e) {
	std::cerr << e.what() << std::endl;
	return;
      }

  std::string dataBaseHashedPasswd = userData.getHashedPasswd();
  std::string receivedHashedPasswd = authPacket.getHashedPasswd();
  std::string salt = user->getDataContainer().getData<char>("SaltKey");

  if (md5(dataBaseHashedPasswd + salt) == receivedHashedPasswd)
    {
      std::cout << "[LogginService::authentificationHandler] account name '"
		<< accountName << "' send a GOOD password." << std::endl;
      user->setAuthentificated(true);
      // TODO remove the salt
      user->getDataContainer().setData("UserData", new UserData(userData));
      if(m_userService)
	m_userService->addConnectedUser(user);
      return ;
    }
  std::cout << "[LogginService::authentificationHandler] account name '"
	    << accountName << "' send a WRONG password." << std::endl;
  user->deliver(new PacketAnswer(PacketAnswer::AUTH_FAILED));
}

void		LogginService::registerHandler(SerializerBuffer *buffer, UserController *user)
{
  PacketRegister registerPacket;

  try
    {
      registerPacket.unserialize(buffer);
    }
  catch (const PacketException &e)
    {
      user->deliver(new PacketAnswer(e.getError()));
      std::cout << "[LogginService::registerHandler] Warning : "
		<< e.what() << std::endl;
      return ;
    }

  std::string accountName = registerPacket.getAccountName();
  if (m_database->accountExistByName(accountName))
    {
      std::cout << "[LogginService::registerHandler] account name '"
		<< accountName << "' is already registered." << std::endl;
      user->deliver(new PacketAnswer(PacketAnswer::REGISTER_ACCOUNT_NAME_TAKEN));
      return;
    }

  try
    {
      uint32_t userId = m_database->createAccount(accountName, registerPacket.getPasswd());
      user->getDataContainer().setData("UserData", new UserData(m_database->getAccountById(userId)));
    }
  catch (const DatabaseException &e)
    {
      std::cout << "[LogginService::registerHandler] Error : Failed to register a user : "
		<< e.what() << std::endl;
      user->deliver(new PacketAnswer(PacketAnswer::REGISTER_ACCOUNT_NAME_TAKEN));
      return ;
    }
  user->setAuthentificated(true);
  user->deliver(new PacketAnswer(PacketAnswer::REGISTER_ACCOUNT_CREATED));
}
