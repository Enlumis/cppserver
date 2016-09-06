#ifndef ADDCONTACTPACKET_H_
# define ADDCONTACTPACKET_H_

# include	<string>
# include	<cstdint>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"

class PacketAddContact : public APacket
{
public:
  PacketAddContact();
  ~PacketAddContact();

private:
  PacketAddContact(const PacketAddContact &);
  PacketAddContact &operator=(const PacketAddContact &);

public:
  void unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer *serialize() const;

  void			setUserId(uint32_t uid);
  uint32_t		getUserId() const;


private:
  uint32_t		m_userID;
};

#endif /* !ADDCONTACTPACKET_H_ */
