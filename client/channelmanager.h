#ifndef CHANNELMANAGER_H
#define CHANNELMANAGER_H

#include <QObject>
#include "Type.hh"

class Convers;
class QMyContact;

class ChannelManager : public QObject
{
  Q_OBJECT

  std::map<uint32_t, Convers*>  _privateChannel;
  std::map<uint32_t, Convers*>  _publicChannel;

public:
  explicit ChannelManager(QObject *parent = 0);
  ~ChannelManager();

private:
  ChannelManager(const ChannelManager &);
  ChannelManager &operator=(const ChannelManager &);

public:
  Convers* CreatePrivateChannel(uint32_t userId);
  void     CreatePublicChannel(uint32_t ChannelId, const std::vector<Type::UserField> &contactList);
  void     removeFromChannel(uint32_t channelId, Type::UserField user);
  void     addUserOnPublicChannel(uint32_t channelId, Type::UserField user);
  bool     isPrivateChannel(uint32_t channelId);
  Convers* getChannelFromId(bool isPublic, uint32_t id);

public:
  void     writeMessageOnPublicChannel(uint32_t channelId, uint32_t srcUserId, std::string const&message);
  void     writeMessageOnPrivateChannel(uint32_t userId, uint32_t srcUserId, std::string const&message);

signals:

public slots:
  void     addContactInThisChannel();
  void     newConversation();

};

#endif // CHANNELMANAGER_H
