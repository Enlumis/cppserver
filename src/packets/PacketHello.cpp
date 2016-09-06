#include  "PacketHello.hh"
#include  "PacketAnswer.hh"
#include  "PacketException.hh"

PacketHello::PacketHello()
  : APacket(APacket::ID_HELLO, APacket::SERVER_TO_CLIENT),
    m_saltSize(32)
{
  generateSalt();
}

PacketHello::~PacketHello()
{}

void PacketHello::unserialize(SerializerBuffer *inBuffer)
{
  try  {
    uint16_t size = inBuffer->readToData<uint16_t>();
    this->m_salt = inBuffer->readToData(size);
    if (m_salt.size() != m_confManager->get<size_t>("saltSize"))
    {
      throw(PacketException(PacketAnswer::HELLO_INVALID_SALT,
          "[PacketHello] failed : account name size to big."));
    }
  } catch (SerializerBufferException &e) {
    std::cerr << e.what() << std::endl;
  }
}

SerializerBuffer *PacketHello::serialize() const
{
  SerializerBuffer *buf = new SerializerBuffer;

  buf->writeToData<uint16_t>(this->m_salt.size());
  buf->writeToData(this->m_salt, this->m_salt.size());

  return (buf);
}

void  PacketHello::generateSalt()
{
  m_salt.resize(m_saltSize);
  for (size_t i = 0; i < m_saltSize; ++i)
    m_salt[i] = rand() % 94 + ' ';
}

const std::string &PacketHello::getSalt() const
{
  return m_salt;
}