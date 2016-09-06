# include           "OpusException.hh"

OpusException::    OpusException(std::string const &error)
    : AudioException("[Opus] : " + error)
{

}

OpusException::    OpusException(char const *error)
    : AudioException("[Opus] : " + std::string(error))
{

}

OpusException::    ~OpusException() throw()
{

}
