#ifndef PACKETSEARCHUSER_H
# define PACKETSEARCHUSER_H

# include	"APacket.hh"
# include	"SerializerBuffer.hpp"
# include	"PacketSearchUser.hh"
# include       <cstdint>


class PacketSearchUser : public APacket
{
private:
  std::string		m_pattern;

public:
  PacketSearchUser();
  virtual ~PacketSearchUser();

private:
  PacketSearchUser(const PacketSearchUser &);
  PacketSearchUser &operator=(const PacketSearchUser &);

public:
  void			unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer*	serialize() const;

  std::string		getPattern() const;
  void			setPattern(const std::string &pattern);
};

#endif // PACKETSEARCHUSER_H
