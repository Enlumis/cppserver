#include	"network/BoostServer.hh"
#include	"packets/SerializerBuffer.hpp"
#include	"loaders/ServiceLoader.hpp"
#include	"services/user-service/UserService.hh"
#include	"packets/PacketHello.hh"
#include	"ServiceManager.hh"
#include	"ConfManager.hh"
#include	<boost/signals2.hpp>
#include	<boost/asio/signal_set.hpp>

int main()
{
  /*
   * TODO : mettre le service loader dans le server ?
   *
   * Limite pourquoi ne pas faire un AServer : qui permettrais de laoder les services ?
   */

  ServiceLoader loader(".");
  ServiceManager *serviceManager = ServiceManager::getInstance();
  ConfManager *confManager = ConfManager::getInstance();

  try {
    IService *service;
    service = loader.loadService<IService>("user-service");
    serviceManager->bindService(service);
    service = loader.loadService<IService>("channel-service");
    serviceManager->bindService(service);
    service = loader.loadService<IService>("call-service");
    serviceManager->bindService(service);
    service = loader.loadService<IService>("loggin-service");
    serviceManager->bindService(service);
    service = loader.loadService<IService>("database-service");
    serviceManager->bindService(service);
  }catch(ClassException &e){
    std::cout << e.what() << std::endl;
    return -1;
  }

  if (!serviceManager->startServices())
  {
	serviceManager->stopServices();
    return -1;
  }

  boost::asio::io_service io_service;
  IServer *boostServer = new BoostServer(io_service);

  boost::asio::signal_set signals(io_service, SIGINT, SIGTERM);
  signals.async_wait(boost::bind(&boost::asio::io_service::stop, &io_service));

  boostServer->run();
  delete(boostServer);
  serviceManager->stopServices();
  return 0;
}
