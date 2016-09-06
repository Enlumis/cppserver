#include	<iostream>
#include	<boost/asio.hpp>
#include	<boost/bind/bind.hpp>
#include	<boost/asio/placeholders.hpp>
#include	"BoostClientSocket.hh"
#include	"../packets/PacketHello.hh"

BoostClientSocket::BoostClientSocket(boost::asio::io_service &io_service)
  : m_serviceManager(ServiceManager::getInstance()),
    m_io_service(io_service),
    m_socket(m_io_service)
{

}

BoostClientSocket::~BoostClientSocket()
{
}


void	BoostClientSocket::onConnected()
{
  recv();

  boost::asio::ip::tcp::endpoint remote_ep = m_socket.remote_endpoint();
  boost::asio::ip::address remote_ad = remote_ep.address();
  m_ipAdress = remote_ad.to_string();
}

int	BoostClientSocket::recv()
{
  char *headerPtr = reinterpret_cast<char*>(&this->m_readHeader);
  boost::asio::async_read(m_socket,
			  boost::asio::buffer(headerPtr, sizeof(struct packetHeader)),
			  boost::bind(&BoostClientSocket::readHeaderHandler, this,
				      boost::asio::placeholders::error));
  return 0;
}

void		BoostClientSocket::readHeaderHandler(const boost::system::error_code& error)
{
  uint16_t	*packetID;
  uint16_t	*packetSize;

  if (!error)
    {
      std::cout << "[BoostClientSocket::readHeaderHandler] Receive Packet id : " << this->m_readHeader.id
      		<< "  size(" << this->m_readHeader.size << ")" << std::endl;
      this->m_dataBuffer = new char[m_readHeader.size];
      boost::asio::async_read(m_socket,
			      boost::asio::buffer(this->m_dataBuffer, m_readHeader.size),
			      boost::bind(&BoostClientSocket::readDataHandler, this,
					  boost::asio::placeholders::error));
    }
  else
    {
      std::cout << "Disconnected !" << std::endl;
      m_userController->disconnect();
    }
}

void		BoostClientSocket::readDataHandler(const boost::system::error_code& error)
{
  if (!error)
    {
      SerializerBuffer *buffer = new SerializerBuffer(this->m_dataBuffer, m_readHeader.size);
      if (!m_serviceManager->handlePacket(m_readHeader.id, buffer, m_userController))
	{
	  std::cout << "[ BoostClientSocket ] : Warning, unknown packet id -> " << m_readHeader.id << std::endl;
	  m_userController->disconnect();
	  return ;
	}
      recv();
    }
  else
    {
      std::cout << "Disconnected !" << std::endl;
      m_userController->disconnect();
    }
  delete[] m_dataBuffer;
}

class ClientSocketException : std::exception
{
public:
  ClientSocketException() throw(){ }
  virtual ~ClientSocketException() throw() {};
  virtual const char* what() const throw()
  {
    return ("Wrong direction exception. ASocket can only send a packet that has a direction : server -> client");
  }
};


int	BoostClientSocket::send(APacket *packet)
{
 std::string data;
  struct packetHeader	*header = new packetHeader;

  if (packet->getDirection() != APacket::SERVER_TO_CLIENT)
    throw ClientSocketException();

  try
    {
      SerializerBuffer *serialized = packet->serialize();
      header->size = serialized->size();
	  std::cout << "bufferSize : " << header->size << std::endl;
      data = serialized->readToData(header->size);
    }
  catch (const SerializerBufferException &e)
    {
      std::cout << e.what() << std::endl;
      return -1;
    }

  header->id = packet->getID();

  std::vector<boost::asio::const_buffer> buffers;

  buffers.push_back(boost::asio::buffer(reinterpret_cast<const char *>(header), sizeof(struct packetHeader)));
  buffers.push_back(boost::asio::buffer(data.c_str(), header->size));

  std::cout << "send packet ID : " << header->id << std::endl;
  std::cout << "send packet size : " << header->size << std::endl;
  //  std::cout << "send packet data : " << data << std::endl;

  boost::asio::async_write(m_socket, buffers,
			   boost::bind(&BoostClientSocket::sendPacketHandler, this,
  				       boost::asio::placeholders::error, packet, header));

  return 0;
}

void	BoostClientSocket::sendPacketHandler(const boost::system::error_code& error,
					     APacket *packetToDelete,
					     struct packetHeader *headerToDelete)
{
  if (!error)
    {
      std::cout << "packet sent !" << std::endl;
    }
  else
    {
      std::cout << "send error, client disconnected !" << std::endl;
      delete packetToDelete;
      delete headerToDelete;
      m_userController->disconnect();
      //delete m_userController;
      return;
    }
  delete packetToDelete;
  delete headerToDelete;
}

void	BoostClientSocket::close()
{
  m_socket.close();
}

boost::asio::ip::tcp::socket &BoostClientSocket::getSocket()
{
  return m_socket;
}

const std::string	&BoostClientSocket::getIpAdress() const
{
  return m_ipAdress;
}
