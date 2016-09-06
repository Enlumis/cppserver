# ifndef    OPUS_EXCEPTION_HH_
# define    OPUS_EXCEPTION_HH_

# include   "AudioException.hh"

class       OpusException : public AudioException
{
public:
  /*!
   * \brief OpusException Constructor.
   *
   * Constructor of the Opus Exception.
   * \param error : The error msg of the exception.
   */
  OpusException(std::string const &error);
  /*!
   * \brief OpusException Constructor.
   *
   * Constructor of the Opus Exception.
   * \param error : The error msg of the exception.
   */
  OpusException(char const *error);
  /*!
   * \brief OpusException Destructor.               
   *                                                
   * Destructor of the Opus Exception.              
   */
  virtual               ~OpusException() throw();
};

# endif                 /* !OPUS_EXCEPTION_HH_ */
