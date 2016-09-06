# ifndef                        APACKET_HH_
# define                        APACKET_HH_

# include                       <string>
# include                       "SerializerBuffer.hpp"
# include                       "../ConfManager.hh"
# include                       "../ConfException.hh"

class                           APacket
{
public:
  typedef enum                  packetID
  {
    ID_ANSWER                   = 0,
    ID_HELLO                    = 1,
    ID_AUTHENTIFICATION         = 2,
    ID_REGISTER                 = 3,
    ID_LOGIN_SUCCESS            = 4,
    ID_ADD_CONTACT              = 10,
    ID_CONTACT_REQUEST          = 11,
    ID_CONTACT_LIST             = 12,
    ID_CONTACT_ADDED            = 13,
    ID_REFUSE_CONTACT           = 14,
    ID_CONTACT_REFUSED          = 15,
    ID_UPDATE_USERNAME          = 20,
    ID_UPDATE_STATUS            = 21,
    ID_USER_EVENT               = 22,
    ID_SEARCH_USER              = 23,
    ID_SEARCH_RESULT            = 24,
    ID_CREATE_CHANNEL           = 30,
    ID_OPEN_CHANNEL             = 31,
    ID_ADD_USER_TO_CHANNEL      = 32,
    ID_CHANNEL_EVENT            = 33,
    ID_SEND_MESSAGE             = 34,
    ID_RECV_MESSAGE             = 35,
    ID_CALL                     = 40,
    ID_ON_CALL                  = 41,
    ID_CALL_JOIN                = 42,
    ID_CALL_ON_JOIN             = 43,
    ID_CALL_QUIT                = 44,
    ID_CALL_ON_QUIT             = 45,
    ID_CALL_STOP                = 46,
    ID_VOICE                    = 1000,
    ID_ON_VOICE                 = 1001
  }           packetID;

  typedef enum                  e_packetDirection
  {
    SERVER_TO_CLIENT,
    CLIENT_TO_SERVER
  }                             packetDirection;

protected:
  packetDirection               m_packetDirection;
  packetID                      m_id;
  ConfManager                   *m_confManager;

public:
  APacket(APacket::packetID, APacket::packetDirection);
  virtual                       ~APacket();

  APacket::packetDirection      getDirection() const;
  APacket::packetID             getID() const;

public:
  virtual void                  unserialize(SerializerBuffer *inBuffer) = 0;
  virtual SerializerBuffer      *serialize() const = 0;
};

# endif                         /* !APACKET_HH_ */
