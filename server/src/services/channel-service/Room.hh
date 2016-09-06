#pragma once

# include	<cstdint>
# include	<string>
# include	<list>

class Room
{
private:
  std::string		_name;
  uint32_t		_id;
  uint32_t		_hostUserId;
  std::list<uint32_t>	_users;

public:
  Room(const std::string &name, uint32_t roomId, const std::list<uint32_t> &users);
  ~Room();

public:
  const std::string		&getName() const;
  uint32_t			getRoomId() const;
  const std::list<uint32_t>	&getUsers() const;
  // uint32_t			getHostUserId() const;
  // void				setHostUserId(uint32_t host);

  bool				isUserInRoom(uint32_t userId) const;
  bool				addUserInRoom(uint32_t newUserId);
  void				removeUserInRoom(uint32_t newUserId);
};
