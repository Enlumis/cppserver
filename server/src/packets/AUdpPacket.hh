
# ifndef                A_UDP_PACKET_HH_
# define                A_UDP_PACKET_HH_

# include               "APacket.hh"
# include               <string>
# include               <cstdint>

class                   AUdpPacket : public APacket
{
public:
    AUdpPacket(APacket::packetID, APacket::e_packetDirection);
    ~AUdpPacket();

private:
    std::string         _destIp;
    uint16_t            _destPort;

public:
    std::string const   &getDestIp() const;
    uint16_t            getDestPort() const;
    void                setDestIp(std::string const &);
    void                setDestPort(uint16_t);

private:
    AUdpPacket(AUdpPacket const &);
    AUdpPacket&         operator=(AUdpPacket const &);

public:
    SerializerBuffer*   serialize() const = 0;
    void                unserialize(SerializerBuffer *) = 0;
};

# endif                 /* !A_UDP_PACKET_HH_ */
