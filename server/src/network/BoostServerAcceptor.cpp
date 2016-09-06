#include	<iostream>
#include	<boost/bind/bind.hpp>
#include	<boost/asio/placeholders.hpp>
#include	"BoostServerAcceptor.hh"
#include	"BoostClientSocket.hh"

BoostServerAcceptor::BoostServerAcceptor(BoostServer &boostServer, short port, boost::asio::io_service &io_service)
  : AServerSocket(port),
    m_boostServer(boostServer),
    m_io_service(io_service),
    m_endpoint(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), _port)),
    m_acceptor(m_io_service, m_endpoint)
{

}

ISocket		*BoostServerAcceptor::accept()
{
  std::cout << "[BoostServerAcceptor] waiting for a new client on port " << _port << " ..." << std::endl;

  BoostClientSocket *clientSocket = new BoostClientSocket(m_io_service);

  m_acceptor.async_accept(clientSocket->getSocket(),
			  boost::bind(&BoostServerAcceptor::handleAccept, this,
				      boost::asio::placeholders::error,
				      clientSocket));
  return NULL;
}

void		BoostServerAcceptor::handleAccept(const boost::system::error_code& error,
						BoostClientSocket *newClient)
{
  if (!error)
    {
      newClient->onConnected();
      m_boostServer.addUser(newClient);
    }
  accept();
}
