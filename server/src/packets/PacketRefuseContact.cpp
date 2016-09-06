#include  "PacketRefuseContact.hh"

PacketRefuseContact::  PacketRefuseContact()
    : APacket(APacket::ID_REFUSE_CONTACT, APacket::CLIENT_TO_SERVER)
{

}

PacketRefuseContact::  ~PacketRefuseContact()
{

}

void                PacketRefuseContact::unserialize(SerializerBuffer *inBuffer)
{
    try {
        m_userID = inBuffer->readToData<uint32_t>();
    } catch (const SerializerBufferException &e) {
        std::cerr   << "[PacketRefuseContact] Error : SerializerBufferException failed : "
                    << e.what() << std::endl;
    }
}

SerializerBuffer    *PacketRefuseContact::serialize() const
{
    SerializerBuffer *out = new SerializerBuffer();

    out->writeToData<uint32_t>(this->m_userID);
    return (out);
}

void                PacketRefuseContact::setUserId(uint32_t uid)
{
    this->m_userID = uid;
}

uint32_t            PacketRefuseContact::getUserId() const
{
    return (this->m_userID);
}
