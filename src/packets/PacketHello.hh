#ifndef PACKETHELLO_H_
# define PACKETHELLO_H_

# include	<string>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"

class PacketHello : public APacket
{
private:
  const size_t	m_saltSize;
  std::string	m_salt;

public:
  PacketHello();
  ~PacketHello();

private:
  PacketHello(const PacketHello &);
  PacketHello &operator=(const PacketHello &);

public:
  void unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer *serialize() const;

  void generateSalt();
  const std::string &getSalt() const;
};

#endif /* !PACKETHELLO_H_ */
