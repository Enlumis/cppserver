#ifndef CONFEXCEPTION_H_
# define CONFEXCEPTION_H_

# include	<exception>
# include	<string>

class ConfException : public std::exception
{
private:
  std::string		m_error;

public:
  ConfException(const std::string &error);
  virtual ~ConfException() throw();

public:
  virtual const char* what() const throw();
};

#endif /* !CONFEXCEPTION_H_ */
