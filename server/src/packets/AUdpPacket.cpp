
# include           "AUdpPacket.hh"

AUdpPacket::        AUdpPacket(APacket::packetID id, APacket::packetDirection dir)
    : APacket(id, dir)
{

}

AUdpPacket::        ~AUdpPacket()
{

}

void                AUdpPacket:: setDestIp(std::string const &ip)
{
    this->_destIp = ip;
}

void                AUdpPacket:: setDestPort(uint16_t port)
{
    this->_destPort = port;
}

std::string const   &AUdpPacket:: getDestIp() const
{
    return (this->_destIp);
}

uint16_t            AUdpPacket:: getDestPort() const
{
    return (this->_destPort);
}