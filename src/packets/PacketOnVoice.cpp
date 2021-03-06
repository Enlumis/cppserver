# include               "PacketOnVoice.hh"

# include               <algorithm>

PacketOnVoice::           PacketOnVoice()
    : AUdpPacket(APacket::ID_ON_VOICE, APacket::SERVER_TO_CLIENT)
{

}

PacketOnVoice::           ~PacketOnVoice()
{

}

void                    PacketOnVoice::setAudioData(AudioData const &data)
{
    this->_data = data;
}

AudioData const         &PacketOnVoice::getAudioData() const
{
    return (this->_data);
}

void                    PacketOnVoice:: setUserId(uint32_t uid)
{
    this->_uid = uid;
}

uint32_t                PacketOnVoice:: getUserId() const
{
    return (this->_uid);
}

void                    PacketOnVoice:: unserialize(SerializerBuffer *in)
{
    try  {
        this->_uid = in->readToData<uint32_t>();
        this->_data.in_size = in->readToData<uint32_t>();
        this->_data.frequency = in->readToData<uint16_t>();
        this->_data.out_size = in->readToData<uint32_t>();
        std::string out = std::string(in->readToData(this->_data.out_size));
        if (this->_data.out_size > 65534)
            throw std::exception();
        this->_data.out = new unsigned char[this->_data.out_size];
        unsigned char *tmp = this->_data.out;
        std::for_each(out.begin(), out.end(), [&tmp](char c) {*tmp = c; tmp++;});
    } catch (SerializerBufferException &e) {
        std::cerr << "[PacketOnVoice] Error : " << e.what() << std::endl;
    }
}

SerializerBuffer        *PacketOnVoice:: serialize() const
{
    SerializerBuffer    *out = new SerializerBuffer();

    out->writeToData<uint32_t>(this->_uid);
    out->writeToData<uint32_t>(this->_data.in_size);
    out->writeToData<uint16_t>(this->_data.frequency);
    out->writeToData<uint32_t>(this->_data.out_size);
    out->writeToData(reinterpret_cast<char * const>(this->_data.out), this->_data.out_size);

    return (out);
}