#include "channelmanager.h"
#include "convers.h"
#include "myqdockwidget.h"
#include "babel.h"
#include "profile.h"
#include "contactlist.h"
#include "Type.hh"
#include "PacketCreateChannel.hh"
#include "qmycontact.h"
#include <QTabBar>

ChannelManager::ChannelManager(QObject *parent) :
  QObject(parent)
{
}

ChannelManager::~ChannelManager()
{}

void			ChannelManager::newConversation()
{
    qDebug() << "new Conversation !";
    ContactList		*contact = bab->getContactList();
    PacketCreateChannel *packet = new PacketCreateChannel();
    packet->addIdToUserId(bab->getProfile()->getId());
    std::list<QMyContact*> const list = contact->getList();
    auto it = list.begin();
    while (it != list.end())
    {
        packet->addIdToUserId((*it)->getId());
        ++it;
    }
    bab->getTSocket()->send(packet);
}

void			ChannelManager::addContactInThisChannel()
{
  QList<QTabBar*>	tabList = bab->findChildren<QTabBar*>();
  if (tabList.empty())
    return ;
  QTabBar		*tabBar = tabList.at(0);
  Convers		*conv = static_cast<Convers*>(bab->findChildren<MyQDockWidget*>().at(tabBar->currentIndex())->widget());
  std::vector<uint32_t> userToAdd;
  std::list<QMyContact*> const& selectedList = bab->getContactList()->getSelectedList();
  qDebug() << " tabBar.count" << tabBar->count();
  for (std::list<QMyContact*>::const_iterator it = selectedList.begin() ; it != selectedList.end() ; it++)
      userToAdd.push_back((*it)->getId());

  if (isPrivateChannel(conv->getId()))
    {
      PacketCreateChannel *packet = new PacketCreateChannel();
      packet->addIdToUserId(bab->getProfile()->getId());
      packet->addIdToUserId(conv->getUserList().front().uid);

      auto it = userToAdd.begin();
      while (it != userToAdd.end())
      {
          packet->addIdToUserId(*it);
          ++it;
      }
      bab->getTSocket()->send(packet);
    }
  else
  {
      auto it = userToAdd.begin();
      qDebug() << "conv->getId = " << conv->getId();
      while (it != userToAdd.end())
      {
        bab->getPacketCreator().Add_user_to_channel(conv->getId(), *it);
        ++it;
      }
  }
}

Convers*		ChannelManager::CreatePrivateChannel(uint32_t userId)
{
  MyQDockWidget		*gui = new MyQDockWidget(bab, (bab->getProfile()->getPseudo()
                                               + bab->getContactList()->getUsernameToId(userId)));
  std::vector<Type::UserField> userList;
  Type::UserField	user;
  user.uid = userId;
  user.username = bab->getContactList()->getUsernameToId(userId).toStdString();
  userList.push_back(user);
  _privateChannel[userId] = new Convers(userId, userList, gui);
  gui->setConvers(_privateChannel[userId]);
  bab->addConvers(gui);
  return (_privateChannel[userId]);
}

void			ChannelManager::CreatePublicChannel(uint32_t ChannelId, std::vector<Type::UserField> const& contactList)
{
  std::string		str;
  std::vector<Type::UserField>::const_iterator it = contactList.begin();
  while (it != contactList.end())
    {
      str += (*it).username;
      it++;
      if (it != contactList.end())
        str += ", ";
    }
  MyQDockWidget		*gui = new MyQDockWidget(bab, QString::fromStdString(str));
  _publicChannel[ChannelId] = new Convers(ChannelId, contactList, gui);
  gui->setConvers(_publicChannel[ChannelId]);
  bab->addConvers(gui);
}

void			ChannelManager::removeFromChannel(uint32_t channelId, Type::UserField user)
{
  _publicChannel[channelId]->removeUserToList(user);
}

void			ChannelManager::addUserOnPublicChannel(uint32_t channelId, Type::UserField user)
{
  _publicChannel[channelId]->addUserToList(user);
  bab->getPacketCreator().Add_user_to_channel(channelId, user.uid);
}

void			ChannelManager::writeMessageOnPublicChannel(uint32_t channelId, uint32_t srcUserId,
								    std::string const& message)
{
  _publicChannel[channelId]->userWriteMessage(srcUserId, message);
}

void			ChannelManager::writeMessageOnPrivateChannel(uint32_t destId, uint32_t srcUserId,
						      std::string const& message)
{
    qDebug() << "[Debug] DestId = " << destId;
    qDebug() << "[Debug] SrcId = " << srcUserId;
    if (destId == bab->getProfile()->getId())
    {
        qDebug() << "Choice 1";
        if (_privateChannel.count(srcUserId) <= 0)
            CreatePrivateChannel(srcUserId);
        _privateChannel[srcUserId]->userWriteMessage(srcUserId, message);
    }
    else
    {
        qDebug() << "Choice 2";
        if (_privateChannel.count(destId) <= 0)
            CreatePrivateChannel(destId);
        _privateChannel[destId]->userWriteMessage(srcUserId, message);
    }
}

bool			ChannelManager::isPrivateChannel(uint32_t channelId)
{
  return (_privateChannel.count(channelId));
}

Convers			*ChannelManager::getChannelFromId(bool isPublic, uint32_t id)
{
  std::map<uint32_t, Convers*> *map = &_privateChannel;

  if (isPublic)
    map = &_publicChannel;
  if (!map->count(id))
    return (NULL);
  return ((*map)[id]);
}
