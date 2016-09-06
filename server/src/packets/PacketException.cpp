#include	"PacketException.hh"

PacketException::PacketException(PacketAnswer::AnswerCode code, const std::string &strError)
  : m_code(code),
    m_strError(strError)
{

}

PacketException::~PacketException() throw()
{

}

PacketAnswer::AnswerCode	PacketException::getError() const
{
  return m_code;
}

const char* PacketException::what() const throw()
{
  return m_strError.c_str();
}
