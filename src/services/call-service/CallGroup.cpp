#include	"CallGroup.hh"

CallGroup::CallGroup()
{
}

CallGroup::~CallGroup()
{

}

uint32_t			CallGroup::getHostUserId() const
{
  return _hostUserId;
}

void			CallGroup::setHostUserId(uint32_t id)
{
  _hostUserId = id;
}

const std::vector<uint32_t>	&CallGroup::getUsers() const
{
  return _users;
}

bool				CallGroup::isUserInCallGroup(uint32_t userId) const
{
  for (std::vector<uint32_t>::const_iterator it = _users.begin();
       it != _users.end();
       ++it)
    {
      if (*it == userId)
	{
	  return true;
	}
    }
  return false;
}

bool				CallGroup::addUserInCallGroup(uint32_t newUserId)
{
  _users.push_back(newUserId);
  return (true);
}

void				CallGroup::removeUserInCallGroup(uint32_t newUserId)
{
  for (std::vector<uint32_t>::iterator it = _users.begin();
       it != _users.end();
       ++it)
    {
      if (*it == newUserId)
	{
	  _users.erase(it);
	  return;
	}
    }
}
