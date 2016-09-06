#ifndef PACKETSENDMESSAGE_H
#define PACKETSENDMESSAGE_H

# include	"APacket.hh"
# include	"SerializerBuffer.hpp"
# include       <cstdint>
# include       <string>

class PacketSendMessage : public APacket
{
public:
  typedef enum	eMessageType
    {
      USER_MESSAGE = 0x00,
      CHANNEL_MESSAGE = 0x01
    }		MessageType;

public:
  PacketSendMessage();
  ~PacketSendMessage();

private:
  PacketSendMessage(const PacketSendMessage &);
  PacketSendMessage &operator=(const PacketSendMessage &);

public:
  void			unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer*	serialize() const;

  uint8_t	getType() const;
  void		setType(uint8_t type);

  uint32_t getChanOrUserId() const;
  void setChanOrUserId(const uint32_t &channelId);

  const std::string &getMessage() const;
  void setMessage(const std::string &message);

private:
  uint32_t	_id;
  std::string	_message;
  uint8_t	_type;
};

#endif // PACKETSENDMESSAGE_H
