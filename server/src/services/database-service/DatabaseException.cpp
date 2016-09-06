#include "DatabaseException.hh"

DatabaseException::DatabaseException(const std::string &what) throw()
{
  this->_what = "' DatabaseException : " + what + " '";
}

DatabaseException::~DatabaseException() throw(){};

const char* DatabaseException::what() const throw()
{
  return (this->_what.c_str());
}
