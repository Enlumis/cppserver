#include "callhost.h"
#include "babel.h"
#include "AudioData.hh"
#include "profile.h"

CallHost::CallHost(QObject *parent) :
  QObject(parent), ICall()
{
}

CallHost::~CallHost()
{}

std::map<uint32_t, std::pair<std::string, uint16_t> > const    &CallHost::getCalledUsers() const
{
    return (this->_usersIp);
}

void      CallHost::addUser(uint32_t userId, std::string const& ip, uint16_t port)
{
    _usersIp[userId] = std::make_pair(ip, port);
}

uint32_t    CallHost::getUserIdFromIp(std::string const &ip) const
{
  auto  it = this->_usersIp.begin();
  while (it != this->_usersIp.end())
  {
      if (it->second.first == ip)
          return (it->first);
      ++it;
  }
    throw std::exception();
}

void      CallHost::removeUser(uint32_t userId)
{
  for (auto it = _usersIp.begin() ; it != _usersIp.end() ; it++)
    if (it->first == userId)
      {
        _usersIp.erase(it);
        break ;
      }
}

void      CallHost::update()
{
  AudioData *data = NULL;
  if (bab->getAudioManager()->toListen(data) == false)
    return ;
  auto it = this->_usersIp.begin();
  while (it != this->_usersIp.end())
  {
    bab->getPacketCreator().OnVoice(bab->getProfile()->getId()
      , *data, it->second.first, it->second.second);
    ++it;
  }
}
