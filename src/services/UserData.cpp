#include "UserData.hh"

UserData::UserData()
{}

UserData::UserData(const UserData &userData)
{
  setStatus(userData.getStatus());
  setUsername(userData.getUsername());
  setHashedPasswd(userData.getHashedPasswd());
  setId(userData.getId());

  const std::vector<uint32_t> &contacts = userData.getContacts();
  for (std::vector<uint32_t>::const_iterator it = contacts.begin();
       it != contacts.end();
       ++it)
    {
      _contacts.push_back(*it);
    }

  const std::vector<uint32_t> &contactsRequest = userData.getContactsRequest();
  for (std::vector<uint32_t>::const_iterator it = contactsRequest.begin();
       it != contactsRequest.end();
       ++it)
    {
      _contactsRequest.push_back(*it);
    }
}

UserData::~UserData()
{}

void	UserData::setId(uint32_t id)
{
  _id = id;
}

void UserData::setUsername(const std::string &username)
{
  this->_username = username;
}

void UserData::setHashedPasswd(const std::string &hashedPasswd)
{
  this->_hashedPasswd = hashedPasswd;
}

void UserData::setStatus(const std::string &status)
{
  this->_status = status;
}

void UserData::addContact(uint32_t contactId)
{
  this->_contacts.push_back(contactId);
}

void	UserData::addContactRequest(uint32_t contactId)
{
  this->_contactsRequest.push_back(contactId);
}
void	UserData::removeContactRequest(uint32_t contactId)
{
  for (size_t i = 0; i < _contactsRequest.size(); ++i)
    {
      if (_contactsRequest[i] == contactId)
	{
	  this->_contactsRequest.erase(this->_contactsRequest.begin() + i);
	  return;
	}
    }
}

void	UserData::setContact(const std::vector<uint32_t> &contactList)
{
  _contacts = contactList;
}

void	UserData::setContactRequest(const std::vector<uint32_t> &contactRequestList)
{
  _contactsRequest = contactRequestList;
}

uint32_t	UserData::getId() const
{
  return _id;
}

const std::string UserData::getUsername() const
{
  return (this->_username);
}

const std::string UserData::getHashedPasswd() const
{
  return _hashedPasswd;
}

const std::string UserData::getStatus() const
{
  return (this->_status);
}

const std::vector<uint32_t> &UserData::getContacts() const
{
  return (this->_contacts);
}

const std::vector<uint32_t> &UserData::getContactsRequest() const
{
  return _contactsRequest;
}

const std::string	&UserData::getSalt() const
{
  return _salt;
}

void UserData::setSalt(std::string salt)
{
  this->_salt = salt;
}

bool	UserData::hasContact(uint32_t contactId) const
{
  for (size_t i = 0; i < _contacts.size(); ++i)
    {
      if (_contacts[i] == contactId)
	return true;
    }
  return false;
}


bool	UserData::hasContactRequest(uint32_t contactId) const
{
  for (size_t i = 0; i < _contactsRequest.size(); ++i)
    {
      if (_contactsRequest[i] == contactId)
	return true;
    }
  return false;
}
