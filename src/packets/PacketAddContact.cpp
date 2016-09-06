#include  "PacketAddContact.hh"

PacketAddContact::  PacketAddContact()
    : APacket(APacket::ID_ADD_CONTACT, APacket::CLIENT_TO_SERVER)
{

}

PacketAddContact::  ~PacketAddContact()
{

}

void                PacketAddContact::unserialize(SerializerBuffer *inBuffer)
{
    try {
        m_userID = inBuffer->readToData<uint32_t>();
    } catch (const SerializerBufferException &e) {
        std::cerr   << "[PacketAddContact] Error : SerializerBufferException failed : "
                    << e.what() << std::endl;
    }
}

SerializerBuffer    *PacketAddContact::serialize() const
{
    SerializerBuffer *out = new SerializerBuffer();

    out->writeToData<uint32_t>(this->m_userID);
    return (out);
}

void                PacketAddContact::setUserId(uint32_t uid)
{
    this->m_userID = uid;
}

uint32_t            PacketAddContact::getUserId() const
{
    return (this->m_userID);
}
