
# include           "AudioException.hh"

AudioException::    AudioException(std::string const &error)
    : _error(error)
{

}

AudioException::    AudioException(char const *error)
    : _error(std::string(error))
{

}

AudioException::    ~AudioException() throw()
{

}

char const          *AudioException::    what() const throw()
{
    std::string error;

    error = "[AudioException] ~> " + this->_error + ".";
    return (error.c_str());
}