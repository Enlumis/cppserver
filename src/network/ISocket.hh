#ifndef ISOCKET_H_
# define ISOCKET_H_

# include	"../packets/APacket.hh"
# include	<cstdint>

class ISocket
{
public:
  /**
   * Read data from the socket.
   * if a full IPackage has been received, then call onPackageReceived(const IPackage &package)
   */
  virtual int			recv() = 0;
  virtual int			send(APacket * package) = 0;
  virtual void			close() = 0;
  virtual ~ISocket() { };
};

#endif /* !ISOCKET_H_ */
