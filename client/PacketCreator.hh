#ifndef   PACKETCREATOR_HH_
# define  PACKETCREATOR_HH_

#include <string>
#include <cstdint>

# include   "AudioData.hh"

class   PacketCreator
{
public:
  PacketCreator();
  ~PacketCreator();

public:
  void    Refuse_contact(uint32_t) const;
  void    Add_contact(uint32_t) const;
  void    Add_user_to_channel(uint32_t channelID, uint32_t newUserId) const;
  void    Send_message(uint8_t type, uint32_t id, std::string const &message) const;
  void    Authenticate(std::string const &salt,  std::string const &username, std::string const &password) const;
  void    Register(std::string const &username, std::string const &password) const;
  void    Voice(AudioData const &, std::string const &, uint16_t) const;
  void    OnVoice(uint32_t, AudioData const &, std::string const &, uint16_t) const;
  void    callJoin(uint32_t userId, uint16_t port) const;
  void    callQuit(uint32_t userId) const;
  void    call(uint32_t userId, uint16_t port, uint8_t comeFrom, uint32_t roomId) const;

};

#endif    /* !PACKETCREATOR_HH_ */
