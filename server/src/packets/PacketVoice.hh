
# ifndef                PACKET_VOICE_HH_
# define                PACKET_VOICE_HH_

# include               "AUdpPacket.hh"
# include               "AudioData.hh"


class                   PacketVoice : public AUdpPacket
{
public:
    PacketVoice();
    ~PacketVoice();

private:
    PacketVoice(PacketVoice const &);
    PacketVoice&        operator=(PacketVoice const &);

private:
    AudioData           _data;

public:
    void                setAudioData(AudioData const &);
    AudioData const     &getAudioData() const;

public:
    void                unserialize(SerializerBuffer *);
    SerializerBuffer*   serialize() const;
};

# endif                 /* !PACKET_VOICE_HH_ */