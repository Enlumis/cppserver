#ifndef UPDATEUSERNAMEPACKET_H_
# define UPDATEUSERNAMEPACKET_H_

# include	<string>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"

class PacketUpdateUsername : public APacket
{
private:
  std::string	m_username;

public:
  PacketUpdateUsername();
  ~PacketUpdateUsername();

private:
  PacketUpdateUsername(const PacketUpdateUsername &);
  PacketUpdateUsername &operator=(const PacketUpdateUsername &);

public:
  void unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer *serialize() const;

  const std::string	&getUsername() const;
  void                   setUsername(std::string const& username);
};

#endif /* !UPDATEUSERNAMEPACKET_H_ */
