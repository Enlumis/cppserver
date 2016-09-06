#include "ClassException.hh"

ClassException::ClassException(const std::string &what) throw()
{
	this->_what = "' ClassException : " + what + " '";
}

ClassException::~ClassException() throw(){};

const char* ClassException::what() const throw()
{
	return (this->_what.c_str());
}