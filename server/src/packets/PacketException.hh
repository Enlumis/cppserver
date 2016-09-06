#ifndef PACKETEXCEPTION_H_
# define PACKETEXCEPTION_H_

# include	<exception>
# include	<string>
# include	"PacketAnswer.hh"

class PacketException : public std::exception
{
private:
  PacketAnswer::AnswerCode	m_code;
  std::string			m_strError;

public:
  PacketException(PacketAnswer::AnswerCode code, const std::string &strError);
  virtual ~PacketException() throw();

  PacketAnswer::AnswerCode	getError() const;

public:
  virtual const char* what() const throw();
};

#endif /* !PACKETEXCEPTION_H_ */
