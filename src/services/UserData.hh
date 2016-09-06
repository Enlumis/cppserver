#pragma once

#include	<string>
#include	<vector>
#include	<boost/cstdint.hpp>

class UserData
{
public:
  UserData();
  UserData(const UserData &userData);
  ~UserData();

public:
  void			setId(uint32_t);
  void			setUsername(const std::string &username);
  void			setHashedPasswd(const std::string &passwd);
  void			setStatus(const std::string &status);
  void			addContact(uint32_t contactId);
  void			addContactRequest(uint32_t contactId);
  void			removeContactRequest(uint32_t contactId);

  void			setContact(const std::vector<uint32_t> &contactList);
  void			setContactRequest(const std::vector<uint32_t> &contactRequestList);

  uint32_t		getId() const;
  const std::string	getUsername() const;
  const std::string	getHashedPasswd() const;
  const std::string	getStatus() const;
  const std::vector<uint32_t> &getContacts() const;
  const std::vector<uint32_t> &getContactsRequest() const;
  const std::string	&getSalt() const;
  void			setSalt(std::string salt);
  bool			hasContact(uint32_t contactId) const;
  bool			hasContactRequest(uint32_t contactId) const;

private:
  uint32_t		_id;
  std::string		_username;
  std::string		_hashedPasswd;
  std::string		_status;
  std::string		_salt;

  std::vector<uint32_t>	_contacts;
  std::vector<uint32_t>	_contactsRequest;
};
