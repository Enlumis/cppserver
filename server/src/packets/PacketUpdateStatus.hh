#ifndef UPDATESTATUSPACKET_H_
# define UPDATESTATUSPACKET_H_

# include	<string>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"

class PacketUpdateStatus : public APacket
{
private:
  std::string	m_status;

public:
  PacketUpdateStatus();
  ~PacketUpdateStatus();

private:
  PacketUpdateStatus(const PacketUpdateStatus &);
  PacketUpdateStatus &operator=(const PacketUpdateStatus &);

public:
  void unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer *serialize() const;

  const std::string	&getStatus() const;
  void                  setStatus(const std::string& status);
};

#endif /* !UPDATESTATUSPACKET_H_ */
