#include	"AClientSocket.hh"

AClientSocket::AClientSocket()
{

}

AClientSocket::~AClientSocket()
{

}

void	AClientSocket::setController(UserController *userController)
{
  m_userController = userController;
}
