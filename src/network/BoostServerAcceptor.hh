#ifndef BOOSTSERVERSOCKET_H_
# define BOOSTSERVERSOCKET_H_

# include	<string>
# include	<boost/system/error_code.hpp>
# include	<boost/asio/ip/tcp.hpp>
# include	"BoostServer.hh"
# include	"AServerSocket.hh"
# include	"ISocket.hh"
# include	"BoostClientSocket.hh"

class BoostServerAcceptor : public AServerSocket
{
private:
  BoostServer				&m_boostServer;
  boost::asio::io_service		&m_io_service;
  boost::asio::ip::tcp::endpoint	m_endpoint;
  boost::asio::ip::tcp::acceptor	m_acceptor;

public:
  BoostServerAcceptor(BoostServer &boostServer, short port, boost::asio::io_service &io_service);
  virtual ~BoostServerAcceptor() {}

public:
  /**
   * Here accept will return NULL, because BoostServerAcceptor
   * is an asynchronous serverSocket so we don't need to return anything.
   * See AServerSocket's documentation for more informations
   */
  virtual ISocket	*accept();

private:
  BoostServerAcceptor(const BoostServerAcceptor &);
  BoostServerAcceptor &operator=(const BoostServerAcceptor &);

private:
  void	handleAccept(const boost::system::error_code& error, BoostClientSocket *);
};

#endif /* !BOOSTSERVERSOCKET_H_ */
