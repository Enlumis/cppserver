#include "convers.h"
#include <QDebug>
#include <QTextCursor>
#include <QLabel>
#include <QMouseEvent>
#include "contactlist.h"
#include "babel.h"
#include "ui_babel.h"
#include <QVBoxLayout>
#include <algorithm>
#include "profile.h"
#include "PacketRecvMessage.hh"
#include "udpqsocket.h"
#include "callhost.h"
#include "CallClient.hh"


Convers::Convers(quint32 id, std::vector<Type::UserField> const& userList, QWidget *parent) :
    QWidget(parent), _layout(new QVBoxLayout(this)), _text(new QTextEdit(this)), _line(new QLineEdit(this)), _id(id)
{
    _userList = userList;
    _callButton = new QPushButton("call", this);
    _callAccept = new QPushButton("accept", this);
    _callRefuse = new QPushButton("refuse", this);
    _callLab = new QLabel("", this);
    _callEnd = new QPushButton("end call", this);
    QWidget       *widget = new QWidget(this);
    QVBoxLayout   *layout = new QVBoxLayout(this);

    _callLab->setHidden(true);
    _callAccept->setHidden(true);
    _callRefuse->setHidden(true);
    _callEnd->setHidden(true);

    layout->addWidget(_callLab);
    layout->addWidget(_callButton);
    layout->addWidget(_callAccept);
    layout->addWidget(_callRefuse);
    layout->addWidget(_callEnd);
    widget->setLayout(layout);
    _layout->addWidget(widget);
    _layout->addWidget(_text);
    _layout->addWidget(_line);
    _layout->setAlignment(Qt::AlignLeft);
    _text->setReadOnly(true);
    setLayout(_layout);

    connect(_line, SIGNAL(returnPressed()), this, SLOT(WriteInText()));
    connect(_line, SIGNAL(focusInEvent()), this, SLOT(lineFocusInEvent()));
    connect(_callButton, SIGNAL(clicked()), this, SLOT(onCallButtonClicked()));
    connect(_callAccept, SIGNAL(clicked()), this, SLOT(onCallAcceptClicked()));
    connect(_callRefuse, SIGNAL(clicked()), this, SLOT(onCallRefuseClicked()));
    connect(_callEnd, SIGNAL(clicked()), this, SLOT(onCallRefuseClicked()));
}

void		Convers::addUserToList(Type::UserField user)
{
  _userList.push_back(user);
  std::string	str;
  for (std::vector<Type::UserField>::iterator it = _userList.begin() ; it != _userList.end() ; it++)
    str += (*it).username;
  dynamic_cast<MyQDockWidget*>(parent())->setWindowTitle(QString::fromStdString(str));
}

void		Convers::removeUserToList(Type::UserField user)
{
  std::string	str;

  auto it = _userList.begin();
  while (it != _userList.end() && it->uid != user.uid)
    it++;
  if (it == _userList.end())
    return ;
  _userList.erase(it);

  for (std::vector<Type::UserField>::iterator it2 = _userList.begin() ; it2 != _userList.end() ; it2++)
    str += it2->username;
  dynamic_cast<MyQDockWidget*>(parent())->setWindowTitle(QString::fromStdString(str));
}

void		Convers::userWriteMessage(uint32_t uid, std::string const &msg)
{
    if (uid == bab->getProfile()->getId())
    {
        qDebug() << "[Debug] userWriteMessage : You write a message : " << msg.c_str();
        _text->append("You: " + QString::fromStdString(msg));
    }
    else
    {
        auto it = this->_userList.begin();

        while (it != this->_userList.end())
        {
            if (it->uid == uid)
            {
                qDebug() << "[Debug] userWriteMessage : " << it->username.c_str() << " write a message : " << msg.c_str();
                _text->append(QString::fromStdString(it->username) + ": " + QString::fromStdString(msg));
                return ;
            }
            ++it;
        }
        if (it == this->_userList.end())
        {
            qDebug() << "[Debug] userWriteMessage : Someone write a message : " << msg.c_str();
            _text->append("Someone: " + QString::fromStdString(msg));
        }
    }
}

quint32		Convers::getId() const
{
  return _id;
}

void		Convers::setId(const quint32 &id)
{
  _id = id;
}
void                                Convers::WriteInText()
{
  if (bab->getChannelManager().isPrivateChannel(_id))
  {
    qDebug() << "[Debug] is on private channel."; 
    bab->getPacketCreator().Send_message(static_cast<uint8_t>(PacketRecvMessage::USER_MESSAGE), _userList[0].uid, _line->text().toStdString());
  }
  else
  {
    qDebug() << "[Debug] is on public channel.";
    bab->getPacketCreator().Send_message(static_cast<uint8_t>(PacketRecvMessage::CHANNEL_MESSAGE), _id, _line->text().toStdString());
  }
  _line->clear();
}

void            Convers::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    {
      static_cast<ContactList*>(bab->getUi()->scrollArea->widget())->clearList();
    }
}

std::vector<Type::UserField> const& Convers::getUserList() const
{
  return _userList;
}

void                                Convers::onCallRecveive(uint32_t uid)
{
  // Appelle quand on recois ON_CALL
  auto it = _userList.begin();
  while (it != _userList.end())
    {
      if (it->uid == uid)
        break;
      it++;
    }
  if (it == _userList.end())
    return ;
  _callLab->setText(QString::fromStdString(it->username) + " is calling you ");
  _callLab->setHidden(false);
  _callButton->setHidden(true);
  _callAccept->setHidden(false);
  _callRefuse->setHidden(false);
}

void            Convers::onCallButtonClicked()
{
  // send le packet CALL pour chaque user dans _userList
  if (bab->getCall() != NULL)
    return ;
  CallHost                        *host = new CallHost;

  bab->setCall(host);
  bab->setUdpSocket(new UdpQSocket(4243));

  if (bab->getThread().isRun() == false)
    bab->getThread().run<AudioManager &>(*bab->getAudioManager());
  // TODO : REMPLIR.
  _callButton->setHidden(true);
  for (auto it = _userList.begin() ; it != _userList.end() ; it++)
    bab->getPacketCreator().call(it->uid, 4243,
                                 !bab->getChannelManager().isPrivateChannel(_id), _id);
}

void            Convers::onCallAcceptClicked()
{
  CallClient    *call = NULL;

  if (bab->getCall() && (call = dynamic_cast<CallClient*>(bab->getCall())) != NULL)
  {
    _callAccept->setHidden(true);
    _callRefuse->setHidden(true);
    _callLab->setHidden(true);
    _callEnd->setHidden(false);
    bab->setUdpSocket(new UdpQSocket(4243));
    bab->getPacketCreator().callJoin(call->getHostId(), 4243);
    bab->getThread().run<AudioManager &>(*bab->getAudioManager());
  }
}


void            Convers::onCallRefuseClicked()
{
  _callAccept->setHidden(true);
  _callRefuse->setHidden(true);
  _callLab->setHidden(true);
  _callButton->setHidden(false);
  _callEnd->setHidden(true);
  bab->endCall();
  bab->getPacketCreator().callQuit(bab->getProfile()->getId());
}
