#include	"CallClient.hh"

#include "babel.h"
#include "AudioData.hh"
#include "profile.h"

CallClient::		CallClient()
{
}

CallClient::		~CallClient()
{
}

void	CallClient::	setHostIp(std::string const &hostIp)
{
  _hostIp = hostIp;
}

std::string const	&CallClient:: getHostIp()
{
  return _hostIp;
}

void                    CallClient::update()
{
    AudioData *data;
    if (bab->getAudioManager()->toListen(data) != true)
        return ;
    bab->getPacketCreator().Voice(*data, this->_hostIp, this->_hostPort);
}

void                    CallClient::setHostId(uint32_t hostId)
{
  _hostId = hostId;
}

uint32_t                CallClient::getHostId() const
{
  return _hostId;
}

uint16_t                CallClient::getHostPort() const
{
  return _hostPort;
}

void                    CallClient::setHostPort(uint16_t port)
{
  _hostPort = port;
}
