#ifndef		_CALLCLIENT_HH_
#define		_CALLCLIENT_HH_
#include	"ICall.hh"

#include <string>
#include <cstdint>

class			CallClient : public ICall
{
public:
  CallClient();
  ~CallClient();

private:
  std::string		_hostIp;
  uint32_t              _hostId;
  uint16_t              _hostPort;

public:
  void			setHostIp(std::string const &hostIp);
  void                  setHostId(uint32_t hostId);
  std::string const	&getHostIp();
  uint32_t              getHostId() const;
  uint16_t              getHostPort() const;
  void                  setHostPort(uint16_t port);
  void                  update();
};

#endif			/* !_CALLCLIENT_HH_ */
