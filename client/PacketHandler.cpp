
# include               "PacketHandler.hh"

# include               "PacketAnswer.hh"
# include               "PacketHello.hh"
# include               "PacketLoginSuccess.hh"
# include               "PacketContactRequest.hh"
# include               "PacketContactList.hh"
# include               "PacketContactAdded.hh"
# include               "PacketContactRefused.hh"
# include               "PacketUserEvent.hh"
# include               "PacketSearchResult.hh"
# include               "PacketOpenChannel.hh"
# include               "PacketChannelEvent.hh"
# include               "PacketSearchUser.hh"
# include               "PacketRecvMessage.hh"
# include               "PacketOnCall.hh"
# include               "PacketCallOnJoin.hh"
# include               "PacketCallOnQuit.hh"
# include               "PacketCallStop.hh"

# include               "PacketVoice.hh"
# include               "PacketOnVoice.hh"

# include               "../server/src/utils/md5.hh"

# include               "PacketRegister.hh"
# include               "PacketAuthentification.hh"

# include               "babel.h"
# include               "ui_babel.h"
# include               "profile.h"
# include               "connectdiag.h"
# include               "contactlist.h"
# include               "addcontact.h"
# include               "channelmanager.h"
# include               "CallClient.hh"
# include               "PacketOnCall.hh"
# include               "udpqsocket.h"
# include               "callhost.h"
# include               "convers.h"
# include               "PacketCallOnQuit.hh"
# include               "AudioException.hh"

# include                <iostream>

PacketHandler::         PacketHandler()
{
    this->_hFunc[APacket::ID_ANSWER]            = &PacketHandler::handleAnswerPacket;
    this->_hFunc[APacket::ID_HELLO]             = &PacketHandler::handleHelloPacket;
    this->_hFunc[APacket::ID_LOGIN_SUCCESS]     = &PacketHandler::handleLoginSuccessPacket;
    this->_hFunc[APacket::ID_CONTACT_REQUEST]   = &PacketHandler::handleContactRequestPacket;
    this->_hFunc[APacket::ID_CONTACT_LIST]      = &PacketHandler::handleContactListPacket;
    this->_hFunc[APacket::ID_CONTACT_ADDED]     = &PacketHandler::handleContactAddedPacket;
    this->_hFunc[APacket::ID_CONTACT_REFUSED]   = &PacketHandler::handleContactRefusedPacket;
    this->_hFunc[APacket::ID_USER_EVENT]        = &PacketHandler::handleUserEventPacket;
    this->_hFunc[APacket::ID_SEARCH_RESULT]     = &PacketHandler::handleSearchResultPacket;
    this->_hFunc[APacket::ID_OPEN_CHANNEL]      = &PacketHandler::handleOpenChannelPacket;
    this->_hFunc[APacket::ID_CHANNEL_EVENT]     = &PacketHandler::handleChannelEventPacket;
    this->_hFunc[APacket::ID_RECV_MESSAGE]      = &PacketHandler::handleRecvMessagePacket;
    this->_hFunc[APacket::ID_ON_CALL]           = &PacketHandler::handleOnCallPacket;
    this->_hFunc[APacket::ID_CALL_ON_JOIN]      = &PacketHandler::handleCallOnJoinPacket;
    this->_hFunc[APacket::ID_CALL_ON_QUIT]      = &PacketHandler::handleCallOnQuitPacket;
}

PacketHandler::         ~PacketHandler()
{

}

bool                    PacketHandler:: handle(APacket::packetID const &id, SerializerBuffer *in) const
{
    std::map<APacket::packetID, handleFunc>::const_iterator it = this->_hFunc.find(id);

    if (it != this->_hFunc.end())
        return ((this->*(it->second))(in));
    return (false);
}

bool                    PacketHandler:: handle(APacket::packetID const &id, SerializerBuffer *in, std::string const &ip) const
{
    if (id == APacket::ID_VOICE)
        return (this->handleVoicePacket(in, ip));
    else
        return (this->handleOnVoicePacket(in, ip));
}

bool                    PacketHandler:: handleHelloPacket(SerializerBuffer *in) const
{
    PacketHello         *packet = new PacketHello();

    try
    {
      packet->unserialize(in);
    } catch(std::exception& e) {
      std::cerr << e.what() << std::endl;
      return false;
    }

    std::string         salt = packet->getSalt();
    ConnectDiag         *diag = bab->getCoDiag();
    std::string         username = diag->getUsername().toStdString();
    std::string         password = diag->getPassword().toStdString();

    bab->getCoDiag()->setSalt(salt);

    if (diag->isRegisterChecked() == true)
        bab->getPacketCreator().Register(username, password);
    else
      bab->getPacketCreator().Authenticate(salt, username, password);
    delete packet;
    return (true);
}

bool                    PacketHandler:: handleLoginSuccessPacket(SerializerBuffer *in) const
{
    PacketLoginSuccess  *packet = new PacketLoginSuccess();

    packet->unserialize(in);
    Profile		*profile = static_cast<Profile*>(bab->getProfile());

    profile->setPseudo(QString::fromStdString(packet->getUsername()));
    profile->setStatus(QString::fromStdString(packet->getStatus()));
    profile->setId(quint32(packet->getUserId()));

    delete packet;
    bab->getCoDiag()->close();
    return (true);
}

bool			PacketHandler:: handleContactRequestPacket(SerializerBuffer *in) const
{
    PacketContactRequest *packet = new PacketContactRequest();

    try {
        packet->unserialize(in);

        ContactList     *contacts = bab->getContactList();
        contacts->addPendingContact(packet->getUserId()
                                    , packet->getUsername()
                                    , packet->getStatus());
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        delete packet;
        return (false);
    }
    delete packet;
    return (true);
}

bool                    PacketHandler:: handleContactListPacket(SerializerBuffer *in) const
{
    PacketContactList   *packet = new PacketContactList();

    packet->unserialize(in);
    QVector<Type::UserField> clist = QVector<Type::UserField>::fromStdVector(packet->getContacts());
    bab->getContactList()->setContactList(clist);
    delete packet;
    return (true);
}

bool                    PacketHandler:: handleContactAddedPacket(SerializerBuffer *in) const
{
    PacketContactAdded  *packet = new PacketContactAdded();

    packet->unserialize(in);

    uint32_t            uid = packet->getUserId();
    ContactList         *clist = bab->getContactList();

    clist->deletePendingContact(uid);
    clist->addContact(packet->getUsername(), packet->getStatus(), uid);

    delete packet;
    return (true);
}

bool                    PacketHandler:: handleContactRefusedPacket(SerializerBuffer *in) const
{
    PacketContactRefused  *packet = new PacketContactRefused();

    packet->unserialize(in);

    uint32_t		uid = packet->getUserId();
    ContactList         *clist = bab->getContactList();

    clist->deletePendingContact(uid);
    
    delete packet;
    return (true);    
}

bool                    PacketHandler::handleOpenChannelPacket(SerializerBuffer *in) const
{
  PacketOpenChannel*    packet = new PacketOpenChannel();

  packet->unserialize(in);
  uint32_t channelId = packet->getChannelId();
  std::vector<Type::UserField> const contacts = packet->getChannelUsers();

  bab->getChannelManager().CreatePublicChannel(channelId, contacts);
  delete packet;
  return (true);
}

bool                    PacketHandler:: handleUserEventPacket(SerializerBuffer *in) const
{
    PacketUserEvent     *packet = new PacketUserEvent();

    packet->unserialize(in);

    uint8_t             eId = packet->getIdEvent();

    Profile* profile = static_cast<Profile*>(bab->getUi()->_profile->widget());
    if (eId == PacketUserEvent::NICKNAME_CHANGED)
    {
        uint32_t        userId = packet->getUserId();
        if (profile->getId() == userId)
            profile->setPseudo(QString::fromStdString(packet->getData()));
        else
            bab->getContactList()->updateContactUsername(packet->getUserId(), packet->getData());
    }
    else if(eId == PacketUserEvent::STATUS_CHANGED)
    {
        uint32_t        userId = packet->getUserId();
        if (profile->getId() == userId)
            profile->setStatus(QString::fromStdString(packet->getData()));
        else
            bab->getContactList()->updateContactStatus(packet->getUserId(), packet->getData());
    }

    delete packet;
    return (true);
}

bool                    PacketHandler:: handleSearchResultPacket(SerializerBuffer *in) const
{
    PacketSearchResult  *packet = new PacketSearchResult();

    packet->unserialize(in);
    std::vector<PacketSearchResult::user_t> const list = packet->getUserList();
    bab->getAddContact()->setSearchResultList(list);

    delete packet;
    return (true);
}

bool                    PacketHandler:: handleChannelEventPacket(SerializerBuffer *in) const
{
    PacketChannelEvent  *packet = new PacketChannelEvent();

    packet->unserialize(in);

    if (packet->getIdEvent() == PacketChannelEvent::USER_LOGGIN)
      bab->getChannelManager().addUserOnPublicChannel(packet->getChannelId(), packet->getUser());
    else
      bab->getChannelManager().removeFromChannel(packet->getChannelId(), packet->getUser());
    delete packet;
    return (true);
}

bool                    PacketHandler:: handleRecvMessagePacket(SerializerBuffer *in) const
{
    PacketRecvMessage   *packet = new PacketRecvMessage();

    packet->unserialize(in);
    if (packet->getType() == PacketRecvMessage::USER_MESSAGE)
        bab->getChannelManager().writeMessageOnPrivateChannel(packet->getChanOrUserId(),
                                packet->getSenderId(), packet->getMessage());
    else
        bab->getChannelManager().writeMessageOnPublicChannel(packet->getChanOrUserId(),
                               packet->getSenderId(), packet->getMessage());
    delete packet;
    return (true);
}

bool                    PacketHandler:: handleOnCallPacket(SerializerBuffer *in) const
{
  PacketOnCall		*packet = new PacketOnCall();

  packet->unserialize(in);
  if (bab->getCall())
  {
      bab->getPacketCreator().callQuit(packet->getUserId());
      return (false);
  }

  CallClient      *client = new CallClient;

  bab->setCall(client);

  client->setHostId(packet->getUserId());
  client->setHostIp(packet->getHostIp());
  client->setHostPort(packet->getPort());

  bool			isPublic = packet->getType();
  uint32_t		id = (isPublic ? packet->getRoomId() : packet->getUserId());
  Convers		*conv = bab->getChannelManager().getChannelFromId(isPublic, id);
  conv->onCallRecveive(packet->getUserId());
  delete packet;
  return (true);
}

bool                    PacketHandler:: handleCallOnJoinPacket(SerializerBuffer *in) const
{
    ICall               *bcall = bab->getCall();

    if (bcall != NULL)
    {
        CallHost        *host = NULL;
        if ((host = dynamic_cast<CallHost*>(bcall)) != NULL)
        {
            PacketCallOnJoin  *packet = new PacketCallOnJoin();
            packet->unserialize(in);
            
            host->addUser(packet->getUserId(), packet->getClientIp(), packet->getClientPort());
            delete packet;
        }
        else
            return (false);
    }
    return (true);
}

bool                    PacketHandler:: handleCallOnQuitPacket(SerializerBuffer *in) const
{
    ICall               *bcall = bab->getCall();

    if (bcall != NULL)
    {
        CallHost        *host = NULL;
        if ((host = dynamic_cast<CallHost*>(bcall)) != NULL)
        {
            PacketCallOnQuit  *packet = new PacketCallOnQuit();
            packet->unserialize(in);

            host->removeUser(packet->getUserId());
            if (host->getCalledUsers().size() <= 0)
                bab->endCall();
            delete packet;
        }
        else
            return (false);
    }
    return (true);
}

bool                    PacketHandler:: handleCallStop(SerializerBuffer *in) const
{
    ICall               *bcall = bab->getCall();

    if (bcall != NULL)
    {
        CallClient	*host = NULL;
        if ((host = dynamic_cast<CallClient*>(bcall)) != NULL)
        {
            PacketCallStop      *packet = new PacketCallStop();
            packet->unserialize(in);
            if (host->getHostId() == packet->getUserId())
              bab->endCall();
            delete packet;
        }
        else
             return (false);
    }
    return (true);
}

bool                    PacketHandler:: handleAnswerPacket(SerializerBuffer *in) const
{
    PacketAnswer        *packet = new PacketAnswer();

    packet->unserialize(in);

    uint16_t code = packet->getCode();
    std::cerr << "[Debug] Code = " << packet->getCode() << std::endl;
    this->_answerHandler.handle(static_cast<APacket::packetID>(code / 100)
        , static_cast<PacketAnswer::AnswerCode>(code));
    delete packet;
    return (true);
}

bool                    PacketHandler::handleVoicePacket(SerializerBuffer *in, std::string const &ip) const
{
    ICall               *bcall = bab->getCall();
    if (bcall != NULL)
    {
        CallHost        *host = NULL;
        if ((host = dynamic_cast<CallHost*>(bcall)) != NULL)
        {
            PacketVoice *packet = new PacketVoice();

            packet->unserialize(in);

            AudioData const data = packet->getAudioData();
            uint32_t    srcId = 0;
            try {
                srcId = host->getUserIdFromIp(ip);
            } catch (std::exception &e) {
                delete packet;
                return (false);
            }
            std::map<uint32_t, std::pair<std::string, uint16_t>> const users = host->getCalledUsers();

            auto it = users.begin();
            while (it != users.end())
            {
                bab->getPacketCreator().OnVoice(srcId, data, it->second.first, it->second.second);
                ++it;
            }
            try {
                AudioManager *audio = bab->getAudioManager();
                if (audio->checkRun() == true)
                {
                    AudioData *listen = new AudioData;
                    listen->in = data.out;
                    listen->in_size = data.out_size;
                    listen->out_size = data.in_size / 4;
                    listen->out = new unsigned char[data.in_size];
                    listen->frequency = data.frequency;
                    audio->toPlayback(listen);
                }
            } catch (AudioException &e) {
                std::cerr << e.what() << std::endl;
                return (false);
            }
            delete packet;
            return (true);
        }
    }
    return (false);
}

bool                    PacketHandler::handleOnVoicePacket(SerializerBuffer *in, std::string const &ip) const
{
    ICall               *bcall = bab->getCall();

    if (bcall != NULL)
    {
        CallClient      *client = NULL;
        if ((client = dynamic_cast<CallClient*>(bcall)) != NULL)
        {
            PacketOnVoice       *packet = new PacketOnVoice();
            
            packet->unserialize(in);

            AudioData const data = packet->getAudioData();

            if (client->getHostIp() != ip)
              {
                delete packet;
                return (false);
              }
            AudioManager *audio = bab->getAudioManager();
            if (audio->checkRun() == true)
            {
                AudioData *listen = new AudioData;
                listen->in = data.out;
                listen->in_size = data.out_size;
                listen->out_size = data.in_size / 4;
                listen->out = new unsigned char[data.in_size];
                listen->frequency = data.frequency;
                audio->toPlayback(listen);
            }

            delete packet;
        }
    }
    return (true);
}
