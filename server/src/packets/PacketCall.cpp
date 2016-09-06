
# include           "PacketCall.hh"

PacketCall::        PacketCall()
  : APacket(APacket::ID_CALL, APacket::CLIENT_TO_SERVER)
  , _room(0)
{

}

PacketCall::        ~PacketCall()
{

}

void                PacketCall::setUserId(uint32_t uid)
{
    this->_uid = uid;
}

uint32_t            PacketCall::getUserId() const
{
    return (this->_uid);
}

void                PacketCall::setPort(uint16_t port)
{
    this->_port = port;
}

uint16_t            PacketCall::getPort() const
{
    return (this->_port);
}

void                PacketCall::setType(uint8_t type)
{
    this->_type = type;
}

uint8_t             PacketCall::getType() const
{
    return (this->_type);
}

void                PacketCall::setRoomId(uint32_t roomId)
{
  _room = roomId;
}

uint32_t            PacketCall::getRoomId() const
{
  return _room;
}

void                PacketCall::unserialize(SerializerBuffer *in)
{
    try {
        this->_uid = in->readToData<uint32_t>();
        this->_port = in->readToData<uint16_t>();
        this->_type = in->readToData<uint8_t>();
        if (static_cast<MessageType>(this->_type) == PacketCall::PUBLIC_CALL)
            this->_room = in->readToData<uint32_t>();
    } catch (SerializerBufferException &e) {
        std::cerr << "[PacketCall] Error : " << e.what() << std::endl;
    }
}

SerializerBuffer    *PacketCall::serialize() const
{
    SerializerBuffer *out = new SerializerBuffer();

    out->writeToData<uint32_t>(this->_uid);
    out->writeToData<uint16_t>(this->_port);
    out->writeToData<uint8_t>(this->_type);
    out->writeToData<uint32_t>(this->_room);

    return (out);
}
