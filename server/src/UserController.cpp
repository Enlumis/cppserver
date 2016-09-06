#include	<iostream>
#include	"UserController.hh"
#include	"../packets/PacketHello.hh"
#include "ServiceManager.hh"

UserController::UserController(AClientSocket *socket)
  : m_socket(socket),
    m_authentificated(false)
{
  m_socket->setController(this);
}

UserController::~UserController()
{
  delete m_socket;
}

bool	UserController::isAuthentificated() const
{
  return m_authentificated;
}

void	UserController::setAuthentificated(bool authentificated)
{
  m_authentificated = authentificated;
}

void	UserController::deliver(APacket *packet) const
{
  m_socket->send(packet);
}

void	UserController::disconnect()
{
  m_socket->close();
  ServiceManager::getInstance()->notifyServerEventClientDisconnected(this);
}

DataContainer	&UserController::getDataContainer()
{
  return m_data;
}

const std::string	&UserController::getIpAdress() const
{
  return m_socket->getIpAdress();
}
