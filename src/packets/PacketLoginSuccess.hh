#ifndef PACKETLOGINSUCCESS_H_
# define PACKETLOGINSUCCESS_H_

# include	<string>
# include	<cstdint>
# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"

class PacketLoginSuccess : public APacket
{
private:
    uint32_t    _uid;
    std::string _username;
    std::string _status;

public:
  PacketLoginSuccess();
  ~PacketLoginSuccess();

  void      setUserData(uint32_t uid, std::string const &username, std::string const &status);

  uint32_t  getUserId() const;
  std::string const &getUsername() const;
  std::string const &getStatus() const;

private:
  PacketLoginSuccess(const PacketLoginSuccess &);
  PacketLoginSuccess &operator=(const PacketLoginSuccess &);

public:
  void unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer *serialize() const;
};

#endif /* !PACKETLOGINSUCCESS_H_ */
