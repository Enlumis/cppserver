
# include           "PortAudioException.hh"

PortAudioException::    PortAudioException(std::string const &error)
    : AudioException("[PortAudio] : " + error)
{

}

PortAudioException::    PortAudioException(char const *error)
    : AudioException("[PortAudio] : " + std::string(error))
{

}

PortAudioException::    ~PortAudioException() throw()
{

}