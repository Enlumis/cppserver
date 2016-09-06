
# ifndef                PACKET_HANDLER_HH_
# define                PACKET_HANDLER_HH_

# include               <map>

# include               "../server/src/packets/APacket.hh"
# include               "../server/src/packets/SerializerBuffer.hpp"
# include               "AnswerHandler.hh"

class                   PacketHandler
{
private:
    AnswerHandler       _answerHandler;

public:
    PacketHandler();
    ~PacketHandler();

private:
    PacketHandler(PacketHandler const &);
    PacketHandler       &operator=(PacketHandler const &);

private:
    typedef bool        (PacketHandler::*handleFunc)(SerializerBuffer *) const;
    std::map<APacket::packetID, handleFunc> _hFunc;
public:
    bool                handle(APacket::packetID const &, SerializerBuffer *) const;
    bool                handle(APacket::packetID const &, SerializerBuffer *, std::string const &) const;
    
public:
    bool                handleAnswerPacket(SerializerBuffer *in) const;
    bool                handleHelloPacket(SerializerBuffer *in) const;
    bool                handleLoginSuccessPacket(SerializerBuffer *in) const;
    bool                handleContactRequestPacket(SerializerBuffer *in) const;
    bool                handleContactListPacket(SerializerBuffer *in) const;
    bool                handleContactAddedPacket(SerializerBuffer *in) const;
    bool                handleContactRefusedPacket(SerializerBuffer *in) const;
    bool                handleOpenChannelPacket(SerializerBuffer *in) const;
    bool                handleUserEventPacket(SerializerBuffer *in) const;
    bool                handleSearchResultPacket(SerializerBuffer *in) const;
    bool                handleChannelEventPacket(SerializerBuffer *in) const;
    bool                handleRecvMessagePacket(SerializerBuffer *in) const;
    bool                handleOnCallPacket(SerializerBuffer *in) const;
    bool                handleCallOnJoinPacket(SerializerBuffer *in) const;
    bool                handleCallStop(SerializerBuffer *in) const;
    bool                handleCallOnQuitPacket(SerializerBuffer *in) const;
    bool                handleOnVoicePacket(SerializerBuffer *in, std::string const &ip) const;
    bool                handleVoicePacket(SerializerBuffer *in, std::string const &ip) const;

};

# endif                 /* !PACKET_HANDLER_HH_ */


