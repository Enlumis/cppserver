#ifndef BOOSTSERVER_H_
# define BOOSTSERVER_H_

# include	<map>
# include	<boost/cstdint.hpp>
# include	<boost/asio/io_service.hpp>
# include	"IServer.hh"

class BoostServer : public IServer
{
private:
  boost::asio::io_service		&m_io_service;

public:
  BoostServer(boost::asio::io_service &io_service);
  virtual ~BoostServer();

  /**
   * See Iserver.hh for the documentations
   */
  virtual void run();

  /**
   * See Iserver.hh for the documentations
   */
  virtual void addUser(AClientSocket *clientToAdd);

private:
  BoostServer(const BoostServer &);
  BoostServer &operator=(const BoostServer &);
};

#endif /* !BOOSTSERVER_H_ */
