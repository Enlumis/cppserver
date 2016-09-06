# ifndef    SERIALIZER_BUFFER_HPP__
# define    SERIALIZER_BUFFER_HPP__

#include <iostream>
#include <string>
#include <sstream>
#include <exception>

class SerializerBufferException : public std::exception
{
  public:
  SerializerBufferException() {}
  virtual ~SerializerBufferException() throw() {};
  virtual const char* what() const throw()
  {
    return ("SerializerBuffer exception");
  }
};

class SerializerBuffer
{
  public:
  SerializerBuffer()
  {
    this->_size = 0;
  }

  SerializerBuffer(const char *data, size_t size)
  {
    this->_stream.write(data, size);
    this->_size = size;
  }

  ~SerializerBuffer(void){}

  public:
  // scalar read
  template <typename T>
  T readToData()
  {
    if (this->_size < sizeof(T))
      throw SerializerBufferException();
    T extractor;

    this->_stream.read(reinterpret_cast<char*>(&extractor), sizeof(T));
    this->_size -= sizeof(T);

    return (extractor);
  }

  // data read
  std::string readToData(size_t size)
  {
    if (this->_size < size)
      throw SerializerBufferException();
    std::string str;
    try{
      str.resize(size);
    } catch (std::bad_alloc&){
      throw SerializerBufferException();
    }
    this->_stream.read(&str[0], size);
    this->_size -= size;
    return (str);
  }

  // scalar write
  template <typename T>
  void writeToData(const T i)
  {
    this->_stream.write(reinterpret_cast<const char*>(&i), sizeof(T));
    this->_size += sizeof(T);
  }

  // data write
  void writeToData(char *cstr, size_t size)
  {
    this->_stream.write(cstr, size);
    this->_size += size;
  }

  // data write
  void writeToData(const char *cstr, size_t size)
  {
    this->_stream.write(cstr, size);
    this->_size += size;
  }

  void writeToData(const std::string &str, size_t size)
  {
    this->_stream.write(str.c_str(), size);
    this->_size += size;
  }

  size_t size()
  {
    return (this->_size);
  }

private:
  std::stringstream _stream;
  size_t _size;
};

# endif     /* !SERIALIZER_BUFFER_HPP__ */