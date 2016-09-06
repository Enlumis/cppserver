#ifndef ACLIENTSOCKET_H_
# define ACLIENTSOCKET_H_

# include	"../UserController.hh"
# include	"ISocket.hh"

class UserController;

class AClientSocket : public ISocket
{
protected:
  UserController	*m_userController;
  std::string		m_ipAdress;

public:
  AClientSocket();
  virtual ~AClientSocket();

  void		setController(UserController *userController);
  virtual const std::string	&getIpAdress() const = 0;

private:
  AClientSocket(const AClientSocket &);
  AClientSocket &operator=(const AClientSocket &);

};

#endif /* !ACLIENTSOCKET_H_ */
