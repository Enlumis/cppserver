#pragma once

#include "APacket.hh"
#include "IService.hh"
#include "UserController.hh"
#include <map>
#include <list>


class ServiceManager
{
private:
  ServiceManager();
  ~ServiceManager();

public:

  static ServiceManager		*getInstance();

  void bindService(IService *service);
  void bindAtRuntimeService(IService *service);
  bool startServices();
  void stopServices();

  IService *getService(const std::string &name);

  bool handlePacket(uint16_t packetID, SerializerBuffer *buffer, UserController *user);
  void notifyServerEventClientConnected(UserController *user);
  void notifyServerEventClientDisconnected(UserController *user);

private:
  void	addPacketIdsFromService(IService *service);
  void	delPacketIdsFromService(IService *service);
  void registerPacket(IService *service, APacket::packetID packetID);
  void unregisterPacket(IService *service, APacket::packetID packetID);

private:
  std::map<APacket::packetID, std::list<IService *> > _mapRegister;
  std::list<IService *> _services;

private:
  static ServiceManager		*m_instance;
};
