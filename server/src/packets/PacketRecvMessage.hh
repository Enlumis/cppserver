#ifndef PACKETRECVMESSAGE_H
#define PACKETRECVMESSAGE_H

# include	"APacket.hh"
# include	"SerializerBuffer.hpp"
# include	"PacketRecvMessage.hh"
# include       <cstdint>

class PacketRecvMessage : public APacket
{
public:
  typedef enum	eMessageType
    {
      USER_MESSAGE = 0x00,
      CHANNEL_MESSAGE = 0x01
    }		MessageType;

public:
  PacketRecvMessage();
  ~PacketRecvMessage();

private:
  PacketRecvMessage(const PacketRecvMessage &);
  PacketRecvMessage &operator=(const PacketRecvMessage &);

public:
  void			unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer*	serialize() const;

  uint32_t	getChanOrUserId() const;
  void		setChanOrUserId(const uint32_t &chanOrUserId);

  uint8_t	getType() const;
  void		setType(uint8_t type);

  uint32_t	getSenderId() const;
  void		setSenderId(const uint32_t &senderId);

  std::string	getMessage() const;
  void		setMessage(const std::string &message);

private:
  uint8_t		_type;
  uint32_t              _id;
  uint32_t              _senderId;
  std::string           _message;
};

#endif // PACKETRECVMESSAGE_H
