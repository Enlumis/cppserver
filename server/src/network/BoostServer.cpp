#include	"BoostServer.hh"
#include	"BoostServerAcceptor.hh"
#include	"../UserController.hh"
#include	<iostream>


BoostServer::BoostServer(boost::asio::io_service &io_service)
  : m_io_service(io_service)
{
}

BoostServer::~BoostServer()
{
}

void	BoostServer::run()
{
  std::cout << "[BoostServer] Starting server ..." << std::endl;
  try
    {
      AServerSocket *server = new BoostServerAcceptor(*this, 4242, m_io_service);
      server->accept();
      m_io_service.run();
      delete(server);
    }
  catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
}

void	BoostServer::addUser(AClientSocket *clientToAdd)
{
  UserController *userController = new UserController(clientToAdd);
  std::cout << "[BoostServer] new client, ip = " << userController->getIpAdress() << std::endl;
	ServiceManager::getInstance()->notifyServerEventClientConnected(userController);
}
