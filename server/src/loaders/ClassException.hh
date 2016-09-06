#pragma once

#include <exception>
#include <string>

class ClassException : public std::exception
{
public:
  ClassException(const std::string &what) throw();
  virtual ~ClassException() throw();
  virtual const char* what() const throw();

private:
	std::string _what;
};