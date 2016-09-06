# ifndef    AUDIO_EXCEPTION_HH_
# define    AUDIO_EXCEPTION_HH_

# include   <exception>
# include   <string>

class       AudioException : public std::exception
{
protected:
    std::string const _error;

public:
  /*!
   * \brief AudioException Constructor.
   *
   * Constructor of the Audio Exception.
   * \param error : The error msg of the exception.
   */
  AudioException(std::string const &error);
  /*!
   * \brief AudioException Constructor.
   *
   * Constructor of the Audio Exception.
   * \param error : The error msg of the exception.
   */
  AudioException(char const *error);
  /*!
   * \brief AudioException Destructor.               
   *                                                
   * Destructor of the Audio Exception.              
   */
  virtual               ~AudioException() throw();

public:
  /*!                                               
   * \brief Error message getter.
   *
   * \return the error message.
   */
  virtual char const    *what() const throw();

};

# endif                 /* !AUDIO_EXCEPTION_HH_ */