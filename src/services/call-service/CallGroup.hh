#pragma once

# include	<cstdint>
# include	<string>
# include	<vector>

class CallGroup
{
private:
  uint32_t		_hostUserId;
  std::vector<uint32_t>	_users;

public:
  CallGroup();
  ~CallGroup();

public:
  const std::vector<uint32_t>	&getUsers() const;

  uint32_t			getHostUserId() const;
  void				setHostUserId(uint32_t host);

  bool				isUserInCallGroup(uint32_t userId) const;
  bool				addUserInCallGroup(uint32_t newUserId);
  void				removeUserInCallGroup(uint32_t newUserId);
};
