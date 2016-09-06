
# ifndef                PACKET_CALL_HH_
# define                PACKET_CALL_HH_

# include	<cstdint>
# include               "APacket.hh"
# include               "SerializerBuffer.hpp"

class                   PacketCall : public APacket
{
public:
    typedef enum        MessageType
    {
        PRIVATE_CALL    = 0x00,
        PUBLIC_CALL     = 0x01
    }                   MessageType;
public:
    PacketCall();
    ~PacketCall();

private:
    PacketCall(PacketCall const &);
    PacketCall&         operator=(PacketCall const &);

public:
    void                unserialize(SerializerBuffer *);
    SerializerBuffer*   serialize() const;

private:
    uint32_t            _uid;
    uint16_t            _port;
    uint8_t             _type;
    uint32_t            _room;

public:
    void                setUserId(uint32_t);
    void                setPort(uint16_t);
    void                setType(uint8_t);
    void                setRoomId(uint32_t);

    uint32_t            getUserId() const;
    uint16_t            getPort() const;
    uint8_t             getType() const;
    uint32_t            getRoomId() const;
};

#endif                 /* !PACKET_CALL_HH_ */