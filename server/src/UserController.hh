#ifndef USERCONTROLLER_H_
# define USERCONTROLLER_H_

# include	<iostream>
# include	<string>
# include	"network/AClientSocket.hh"
# include	"packets/APacket.hh"
# include	"DataContainer.hpp"

class AClientSocket;

class UserController
{
private:
  AClientSocket		*m_socket;
  bool			m_authentificated;
  DataContainer		m_data;

public:
  UserController(AClientSocket *socket);
  virtual ~UserController();

  bool		isAuthentificated() const;
  void		setAuthentificated(bool authentificated);

  void		deliver(APacket * packet) const;
  void		disconnect();

  DataContainer	&getDataContainer();

  const std::string	&getIpAdress() const;

private:
  UserController(const UserController &);
  UserController &operator=(const UserController &);
};

#endif /* !USERCONTROLLER_H_ */
