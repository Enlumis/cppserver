# ifndef    PORT_AUDIO_EXCEPTION_HH_
# define    PORT_AUDIO_EXCEPTION_HH_

# include   "AudioException.hh"

class       PortAudioException : public AudioException
{
public:
  /*!
   * \brief PortAudioException Constructor.
   *
   * Constructor of the PortAudio Exception.
   * \param error : The error msg of the exception.
   */
  PortAudioException(std::string const &error);
  /*!
   * \brief PortAudioException Constructor.
   *
   * Constructor of the PortAudio Exception.
   * \param error : The error msg of the exception.
   */
  PortAudioException(char const *error);
  /*!
   * \brief PortAudioException Destructor.               
   *                                                
   * Destructor of the PortAudio Exception.              
   */
  virtual               ~PortAudioException() throw();
};

# endif                 /* !PORT_AUDIO_EXCEPTION_HH_ */