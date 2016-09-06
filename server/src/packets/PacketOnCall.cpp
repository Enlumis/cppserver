
# include           "PacketOnCall.hh"

PacketOnCall::        PacketOnCall()
  : APacket(APacket::ID_ON_CALL, APacket::SERVER_TO_CLIENT)
  , _room(0)
{

}

PacketOnCall::        ~PacketOnCall()
{

}

void                PacketOnCall::setUserId(uint32_t uid)
{
    this->_uid = uid;
}

uint32_t            PacketOnCall::getUserId() const
{
    return (this->_uid);
}

void                PacketOnCall::setPort(uint16_t port)
{
    this->_port = port;
}

uint16_t            PacketOnCall::getPort() const
{
    return (this->_port);
}

void                PacketOnCall::setType(uint8_t type)
{
    this->_type = type;
}

uint8_t             PacketOnCall::getType() const
{
    return (this->_type);
}

void                PacketOnCall::setHostIp(std::string const &host)
{
    this->_ip = host;
}

std::string const   &PacketOnCall::getHostIp() const
{
    return (this->_ip);
}

void                PacketOnCall::setRoomId(uint32_t roomId)
{
  _room = roomId;
}

uint32_t            PacketOnCall::getRoomId() const
{
  return _room;
}

void                PacketOnCall::unserialize(SerializerBuffer *in)
{
    try {
        this->_uid = in->readToData<uint32_t>();
        uint16_t size = in->readToData<uint16_t>();
        this->_ip = std::string(in->readToData(size));
        this->_port = in->readToData<uint16_t>();
        this->_type = in->readToData<uint8_t>();
        if (static_cast<MessageType>(this->_type) == PacketOnCall::PUBLIC_CALL)
            this->_room = in->readToData<uint32_t>();
    } catch (SerializerBufferException &e) {
        std::cerr << "[PacketOnCall] Error : " << e.what() << std::endl;
    }
}

SerializerBuffer    *PacketOnCall::serialize() const
{
    SerializerBuffer *out = new SerializerBuffer();

    out->writeToData<uint32_t>(this->_uid);
    out->writeToData<uint16_t>(this->_ip.size());
    out->writeToData(this->_ip, this->_ip.size());
    out->writeToData<uint16_t>(this->_port);
    out->writeToData<uint8_t>(this->_type);
    out->writeToData<uint32_t>(this->_room);

    return (out);
}
