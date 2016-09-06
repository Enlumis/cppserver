
# ifndef                PACKET_ON_VOICE_HH_
# define                PACKET_ON_VOICE_HH_

# include               "AUdpPacket.hh"
# include               "AudioData.hh"

class                   PacketOnVoice : public AUdpPacket
{
public:
    PacketOnVoice();
    ~PacketOnVoice();

private:
    PacketOnVoice(PacketOnVoice const &);
    PacketOnVoice&        operator=(PacketOnVoice const &);

private:
    uint32_t            _uid;
    AudioData           _data;

public:
    void                setUserId(uint32_t);
    uint32_t            getUserId() const;
    void                setAudioData(AudioData const &);
    AudioData const     &getAudioData() const;

public:
    void                unserialize(SerializerBuffer *);
    SerializerBuffer*   serialize() const;
};

# endif                 /* !PACKET_ON_VOICE_HH_ */