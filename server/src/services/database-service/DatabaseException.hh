#pragma once

#include <exception>
#include <string>

class DatabaseException : std::exception
{
public:
  DatabaseException(const std::string &what) throw();
  virtual ~DatabaseException() throw();
  virtual const char* what() const throw();

private:
	std::string _what;
};