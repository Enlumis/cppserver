# ifndef                PACKET_CONTACT_LIST_HH_
# define                PACKET_CONTACT_LIST_HH_

# include               <string>
# include               <vector>
# include               <cstdint>

# include               "SerializerBuffer.hpp"
# include               "APacket.hh"
# include               "Type.hh"

class                   PacketContactList : public APacket
{
private:
    std::vector<Type::UserField>    _contacts;

public:
    PacketContactList();
    ~PacketContactList();

private:
    PacketContactList(PacketContactList const &);
    PacketContactList   &operator=(PacketContactList const &);

public:
    void                unserialize(SerializerBuffer *);
    SerializerBuffer    *serialize() const;

public:
    void                addContact(uint32_t, std::string const &, std::string const &);
    const std::vector<Type::UserField>   &getContacts() const;
};

# endif                 /* !PACKET_CONTACT_LIST_HH_ */
