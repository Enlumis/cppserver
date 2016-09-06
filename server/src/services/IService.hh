#ifndef ISERVICE_H_
# define ISERVICE_H_

# include	<list>
# include	<string>
# include	"../packets/APacket.hh"
# include	"../UserController.hh"

class ServiceManager;

class IService
{
public:
	virtual void		handlePacket(APacket::packetID packetid, SerializerBuffer *buffer, UserController *user) = 0;
	virtual bool		startService(ServiceManager *serviceManager) = 0;
  virtual void		stopService() = 0;

  virtual std::string	getServiceName() const = 0;
  virtual const std::list<APacket::packetID>	&getPacketIds() const = 0;

virtual void onServerEventClientDisconnected(UserController *user) = 0;
virtual void onServerEventClientConnected(UserController *user) = 0;
};

#endif /* !ISERVICE_H_ */
