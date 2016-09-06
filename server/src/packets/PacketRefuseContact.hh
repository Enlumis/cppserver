# ifndef            PACKET_REFUSE_CONTACT_HH_
# define            PACKET_REFUSE_CONTACT_HH_

# include           <string>
# include           <cstdint>
# include           "../packets/APacket.hh"
# include           "../packets/SerializerBuffer.hpp"

class               PacketRefuseContact : public APacket
{
public:
    PacketRefuseContact();
    ~PacketRefuseContact();

private:
    PacketRefuseContact(const PacketRefuseContact &);
    PacketRefuseContact &operator=(const PacketRefuseContact &);

public:
    void unserialize(SerializerBuffer *inBuffer);
    SerializerBuffer *serialize() const;

    void            setUserId(uint32_t uid);
    uint32_t        getUserId() const;


private:
    uint32_t        m_userID;
};

# endif             /* !PACKET_REFUSE_CONTACT_HH_ */
