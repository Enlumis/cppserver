#include	"PacketAnswer.hh"

PacketAnswer::PacketAnswer(uint16_t code)
  : APacket(APacket::ID_ANSWER, APacket::SERVER_TO_CLIENT),
    m_code(code)
{

}

void	PacketAnswer::unserialize(SerializerBuffer *inBuffer)
{
  try {
    m_code = inBuffer->readToData<uint16_t>();
  } catch (const SerializerBufferException &e) {
    std::cerr   << "[PacketAnswer] Error : SerializerBufferException failed : "
                << e.what() << std::endl;
  }
}

SerializerBuffer	*PacketAnswer::serialize() const
{
  SerializerBuffer *buf = new SerializerBuffer(reinterpret_cast<const char *>(&m_code),
					       sizeof(uint16_t));
  return (buf);
}

uint16_t PacketAnswer::getCode() const
{
  return m_code;
}

void PacketAnswer::setCode(const e_answerCode code)
{
  m_code = static_cast<uint16_t>(code);
}
