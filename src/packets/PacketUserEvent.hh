#ifndef PACKETUSEREVENT_HH
#define PACKETUSEREVENT_HH

# include	<string>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"
# include       <cstdint>

class PacketUserEvent : public APacket
{
public:
  typedef enum        EventType {
    NICKNAME_CHANGED  = 0x00,
    STATUS_CHANGED    = 0x01
  }                   EventType;

private:
  uint32_t      _userId;
  uint8_t       _idEvent;
  std::string   _data;

public:
  PacketUserEvent();
  ~PacketUserEvent();
private:
  PacketUserEvent(const PacketUserEvent &);
  PacketUserEvent &operator=(const PacketUserEvent &);

public:
  void			unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer	*serialize() const;

  uint32_t	getUserId() const;
  void		setUserId(uint32_t userId);

  uint8_t	getIdEvent() const;
  void		setIdEvent(uint8_t idEvent);

  std::string	getData() const;
  void		setData(const std::string &data);
};

#endif // PACKETUSEREVENT_HH
