#include	"Room.hh"

Room::Room(const std::string &name, uint32_t roomId, const std::list<uint32_t> &users)
  : _name(name),
    _id(roomId),
    _users(users)
{
}

Room::~Room()
{

}

const std::string		&Room::getName() const
{
  return _name;
}

uint32_t			Room::getRoomId() const
{
  return _id;
}

// uint32_t			Room::getHostUserId() const
// {
//   return _hostUserId;
// }

// void			Room::setHostUserId(uint32_t id)
// {
//   _hostUserId = id;
// }

const std::list<uint32_t>	&Room::getUsers() const
{
  return _users;
}

bool				Room::isUserInRoom(uint32_t userId) const
{
  for (std::list<uint32_t>::const_iterator it = _users.begin();
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

bool				Room::addUserInRoom(uint32_t newUserId)
{
  _users.push_front(newUserId);
  return (true);
}

void				Room::removeUserInRoom(uint32_t newUserId)
{
	for (std::list<uint32_t>::iterator it = _users.begin();
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
