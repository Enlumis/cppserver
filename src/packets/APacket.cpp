# include "../packets/APacket.hh"

APacket::APacket(APacket::packetID packetID, APacket::packetDirection packetDirection)
  : m_packetDirection(packetDirection),
    m_id(packetID),
    m_confManager(ConfManager::getInstance())
{

}

APacket::~APacket()
{}

APacket::packetDirection	APacket::getDirection() const
{
  return m_packetDirection;
}

APacket::packetID		APacket::getID() const
{
  return m_id;
}
