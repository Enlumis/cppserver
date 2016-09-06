#ifndef PACKETCHANNELEVENT_H
#define PACKETCHANNELEVENT_H

# include	<cstdint>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"
# include	"Type.hh"

class PacketChannelEvent : public APacket
{
public:
  typedef enum        EventType {
    USER_LOGGIN = 0x00,
    USER_LOGGOUT = 0x01
  }                   EventType;

public:
  PacketChannelEvent();
  ~PacketChannelEvent();

private:
  PacketChannelEvent(const PacketChannelEvent &);
  PacketChannelEvent &operator=(const PacketChannelEvent &);

public:
  void            unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer *serialize() const;

  uint32_t	getChannelId() const;
  void		setChannelId(const uint32_t &channelId);

  uint8_t	getIdEvent() const;
  void		setIdEvent(uint8_t event);

  uint32_t	getUserId() const;
  void		setUserId(const uint32_t &userId);

  const std::string	&getUsername() const;
  void			setUsername(const std::string &username);

  const std::string	&getStatus() const;
  void			setStatus(const std::string &status);

  const Type::UserField	&getUser();
  void			setUser(Type::UserField const &user);
private:
  uint32_t		_channelId;
  uint8_t		_idEvent;
  Type::UserField	_user;
};

#endif // PACKETCHANNELEVENT_H
