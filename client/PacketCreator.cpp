#include "PacketCreator.hh"
#include "PacketAddContact.hh"
#include "PacketRefuseContact.hh"
#include "PacketAddUserToChannel.hh"
#include "PacketSendMessage.hh"
#include "PacketRegister.hh"
#include "PacketAuthentification.hh"
#include "babel.h"
#include "profile.h"
#include "PacketCallJoin.hh"
#include "PacketCallQuit.hh"
#include "PacketVoice.hh"
#include "PacketOnVoice.hh"
#include "PacketCall.hh"

#include "../server/src/utils/md5.hh"

/*
*************
  add_user_to_channel va potentiellement devenir add_contact_to_channel
*************
*/

PacketCreator::             PacketCreator()
{

}

PacketCreator::             ~PacketCreator()
{

}

void PacketCreator::        Add_contact(uint32_t uid) const
{
  PacketAddContact          *packet = new PacketAddContact();

  packet->setUserId(uid);
  bab->getTSocket()->send(packet);
  delete packet;
}

void PacketCreator::        Refuse_contact(uint32_t uid) const
{
  PacketRefuseContact       *packet = new PacketRefuseContact();

  packet->setUserId(uid);
  bab->getTSocket()->send(packet);
  delete packet;
}

void PacketCreator::        Add_user_to_channel(uint32_t channelID, uint32_t newUserId) const
{
  PacketAddUserToChannel    *packet = new PacketAddUserToChannel();

  packet->setChannelId(channelID);
  packet->setNewUserId(newUserId);
  bab->getTSocket()->send(packet);
  delete packet;
}

void PacketCreator::        Send_message(uint8_t type, uint32_t id, std::string const &message) const
{
  PacketSendMessage         *packet = new PacketSendMessage();

  packet->setChanOrUserId(id);
  packet->setMessage(message);
  packet->setType(type);
  bab->getTSocket()->send(packet);
  delete packet;
}

void PacketCreator::        Register(std::string const &username, std::string const &password) const
{
  PacketRegister            *packet = new PacketRegister();

  packet->setAccountName(username);
  packet->setPasswd(MD5(password).hexdigest());
  bab->getTSocket()->send(packet);
  delete packet;
}

void PacketCreator::        Authenticate(std::string const &salt, std::string const &username, std::string const &password) const
{
  PacketAuthentification    *packet = new PacketAuthentification();
  std::string               HashedPassword(MD5(password).hexdigest() + salt);

  HashedPassword = MD5(HashedPassword).hexdigest();
  packet->setAccountName(username);
  packet->setHashedPasswd(HashedPassword);
  bab->getTSocket()->send(packet);
  delete packet;
}

void PacketCreator::        Voice(AudioData const &data, std::string const &ip, uint16_t port) const
{
  PacketVoice               *packet = new PacketVoice();

  packet->setAudioData(data);
  packet->setDestIp(ip);
  packet->setDestPort(port);
  bab->getUdpSocket()->send(packet);
  delete packet;  
}

void PacketCreator::        OnVoice(uint32_t uid, AudioData const &data, std::string const &ip, uint16_t port) const
{
  PacketOnVoice             *packet = new PacketOnVoice();

  packet->setUserId(uid);
  packet->setAudioData(data);
  packet->setDestIp(ip);
  packet->setDestPort(port);
  bab->getUdpSocket()->send(packet);
  delete packet;
}

void PacketCreator::        callJoin(uint32_t userId, uint16_t port) const
{
  PacketCallJoin            *packet = new PacketCallJoin();

  packet->setUserId(userId);
  packet->setPort(port);
  bab->getTSocket()->send(packet);
  delete packet;
}

void PacketCreator::        callQuit(uint32_t userId) const
{
  PacketCallQuit            *packet = new PacketCallQuit();

  packet->setUserId(userId);
  bab->getTSocket()->send(packet);
  delete packet;
}

void PacketCreator::        call(uint32_t userId, uint16_t port, uint8_t comeFrom, uint32_t roomId) const
{
  PacketCall                *packet = new PacketCall();

  packet->setUserId(userId);
  packet->setPort(port);
  packet->setType(comeFrom);
  packet->setRoomId(roomId);
  bab->getTSocket()->send(packet);
  delete packet;
}
