#ifndef ISERVER_H_
# define ISERVER_H_

# include	"AClientSocket.hh"
# include	"../UserController.hh"

class IServer
{
public:
  virtual ~IServer() {}

  /**
   * This method is the main loop.
   *
   * With this method it is possible to implement synchronous and asynchronous server / client listener.
   * For a synchronous server / client it will handler the call of select (by example).
   / For a asynchronous server /client it will just start the wait_for_* methods.
   */
  virtual void	run() = 0;

  /**
   * This method is called by an asynchronous and a synchronous server.
   * It create a new UserController and bind it to an ISocket.
   */
  virtual void addUser(AClientSocket *clientToAdd) = 0;
};

#endif /* !ISERVER_H_ */
