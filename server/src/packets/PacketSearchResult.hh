#ifndef PACKETSEARCHRESULT_H
#define PACKETSEARCHRESULT_H

# include	"../packets/APacket.hh"
# include	"../packets/SerializerBuffer.hpp"
# include       <cstdint>
# include       <vector>

class PacketSearchResult : public APacket
{
public:
  struct user_t
  {
    uint32_t		  userId;
    std::string		userName;
    std::string		status;
  };

private:
  std::vector<PacketSearchResult::user_t>	m_userList;

public:
  PacketSearchResult();
  ~PacketSearchResult();

private:
  PacketSearchResult(const PacketSearchResult &);
  PacketSearchResult &operator=(const PacketSearchResult &);

public:
  void  unserialize(SerializerBuffer *inBuffer);
  SerializerBuffer* serialize() const;

  void  addUserInlist(uint32_t userId, const std::string &username, const std::string &status);
  const std::vector<PacketSearchResult::user_t>	&getUserList() const;
};

#endif // PACKETSEARCHRESULT_H
