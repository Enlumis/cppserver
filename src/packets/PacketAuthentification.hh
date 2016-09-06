#ifndef AUTHENTIFICATIONPACKET_H_
# define AUTHENTIFICATIONPACKET_H_

# include	<string>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"

class PacketAuthentification : public APacket
{
public:
  PacketAuthentification();
  ~PacketAuthentification();

private:
  PacketAuthentification(const PacketAuthentification &);
  PacketAuthentification &operator=(const PacketAuthentification &);

public:
  void unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer *serialize() const;

  const std::string	&getAccountName() const;
  const std::string	&getHashedPasswd() const;

  void  setAccountName(std::string const &);
  void  setHashedPasswd(std::string const &);

private:
  std::string	m_accountName;
  std::string	m_hashedPasswd;
};

#endif /* !AUTHENTIFICATIONPACKET_H_ */
