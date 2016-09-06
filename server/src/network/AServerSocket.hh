#ifndef ASERVERSOCKET_H_
# define ASERVERSOCKET_H_

# include	<string>
# include	"ISocket.hh"

class AServerSocket
{
protected:
  const short		_port;

public:
  AServerSocket(short port);
  virtual ~AServerSocket() {}

  /**
   * accept a new client.
   * For an asynchronous server it will probably return NULL.
   * For a synchronous server it return a pointer to an ISocket.
   */
  virtual ISocket *accept() = 0;
};

#endif /* !ASERVERSOCKET_H_ */
