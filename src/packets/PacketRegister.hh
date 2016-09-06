#ifndef REGISTERPACKET_H_
# define REGISTERPACKET_H_

# include	<string>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"

class PacketRegister : public APacket
{
public:
  PacketRegister();
  ~PacketRegister();

private:
  PacketRegister(const PacketRegister &);
  PacketRegister &operator=(const PacketRegister &);

public:
  void unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer *serialize() const;

  const std::string	&getAccountName() const;
  const std::string	&getPasswd() const;
  void                   setAccountName(std::string const& accountName);
  void                   setPasswd(std::string const& passwd);

private:
  std::string	m_accountName;
  std::string	m_passwd;
};

#endif /* !REGISTERPACKET_H_ */
