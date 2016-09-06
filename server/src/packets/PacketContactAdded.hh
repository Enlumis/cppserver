# ifndef            PACKET_CONTACT_ADDED_HH_
# define            PACKET_CONTACT_ADDED_HH_

# include           <string>
# include           <cstdint>
# include           "../packets/APacket.hh"
# include           "../packets/SerializerBuffer.hpp"

# include           "Type.hh"


class               PacketContactAdded : public APacket
{
public:
    PacketContactAdded();
    ~PacketContactAdded();

private:
    PacketContactAdded(const PacketContactAdded &);
    PacketContactAdded &operator=(const PacketContactAdded &);

public:
    SerializerBuffer *serialize() const;
    void            unserialize(SerializerBuffer *inBuffer);

public:
    void            setUserData(uint32_t, std::string const &, std::string const &);
    uint32_t        getUserId() const;
    std::string const &getUsername() const;
    std::string const &getStatus() const;



private:
    Type::UserField       _contact;
};

# endif             /* !PACKET_CONTACT_ADDED_HH_ */
