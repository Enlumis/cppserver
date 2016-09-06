#include	"ConfException.hh"

ConfException::ConfException(const std::string &error)
  : m_error(error)
{

}

ConfException::~ConfException() throw()
{

}

const char* ConfException::what() const throw()
{
  return m_error.c_str();
}
