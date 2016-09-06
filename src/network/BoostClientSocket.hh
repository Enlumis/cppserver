#ifndef BOOSTCLIENTSOCKET_H_
# define BOOSTCLIENTSOCKET_H_

# include	<boost/asio/ip/tcp.hpp>
# include	"ISocket.hh"
# include	"../UserController.hh"
# include	"../services/ServiceManager.hh"
# include	"AClientSocket.hh"

class BoostClientSocket : public AClientSocket
{
protected:
  struct packetHeader
  {
    uint16_t id;
    uint16_t size;
  };

private:
  ServiceManager			*m_serviceManager;
  boost::asio::io_service		&m_io_service;
  boost::asio::ip::tcp::socket		m_socket;
  struct packetHeader			m_readHeader;
  char					*m_dataBuffer;


public:
  BoostClientSocket(boost::asio::io_service &io_service);
  virtual ~BoostClientSocket();

  virtual int	recv();
  virtual int	send(APacket * package);
  virtual void	close();
  virtual const std::string	&getIpAdress() const;

  boost::asio::ip::tcp::socket &getSocket();
  void				onConnected();

private:
  void		readHeaderHandler(const boost::system::error_code& error);
  void		readDataHandler(const boost::system::error_code& error);
  void		sendPacketHandler(const boost::system::error_code& error,
				  APacket *packetToDelete,
				  struct packetHeader *headerToDelete);

private:
  BoostClientSocket(const BoostClientSocket &);
  BoostClientSocket &operator=(const BoostClientSocket &);
};

#endif /* !BOOSTCLIENTSOCKET_H_ */
