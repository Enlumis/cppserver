#include "qmycontact.h"
#include "ui_qmycontact.h"
#include "ui_babel.h"
#include "babel.h"
#include "convers.h"
#include "myqdockwidget.h"
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>
#include <QShortcut>
#include "config.h"
#include "guistrstyle.h"
#include "PacketCreateChannel.hh"
#include "profile.h"

extern GuiStrStyle *guiStyle;

QMyContact::QMyContact(QString const& str, QWidget *parent) :
  QFrame(parent), _conv(NULL), _parent(static_cast<ContactList*>(parent)),
  ui(new Ui::QMyContact)
{
  _id = 0;
  _isSelected = false;
  ui->setupUi(this);
  ui->_login->setText(str);
  ui->_coImg->setHidden(true);
//  ui->_accept->setHidden(true);
//  ui->_refuse->setHidden(true);
  ui->_accept->setHidden(true);
  ui->_refuse->setHidden(true);
  this->setObjectName("test");
  connect(new QShortcut(QKeySequence(Qt::Key_D), this), SIGNAL(activated()), _parent, SLOT(downContact()));
  connect(guiStyle, SIGNAL(selectContactChanged()), this, SLOT(onSelectContactChanged()));
  connect(ui->_accept, SIGNAL(isClicked()), this, SLOT(acceptClicked()));
  connect(ui->_refuse, SIGNAL(isClicked()), this, SLOT(refuseClicked()));
  //  QTimer::singleShot(3000, this, SLOT(isConnected()));
}
void		QMyContact::setUserName(QString const& username)
{
  ui->_login->setText(username);
}

QString		QMyContact::getUserName() const
{
  return ui->_login->text();
}

void		QMyContact::setStatus(QString const& status)
{
  _status = status;
}

QString const&	QMyContact::getStatus() const
{
  return _status;
}

void            QMyContact::setId(quint32 id)
{
  _id = id;
}

quint32         QMyContact::getId() const
{
  return _id;
}

void		QMyContact::onSelectContactChanged()
{
  if (_isSelected == true)
    setStyleSheet(guiStyle->selectContact());
}


void		QMyContact::isDisconnected()
{
//  QTimer::singleShot(3000, this, SLOT(isAfk()));
}

void		QMyContact::isConnected()
{
//  QTimer::singleShot(3000, this, SLOT(isDisconnected()));
}

void		QMyContact::isAfk()
{
//  QTimer::singleShot(3000, this, SLOT(isConnected()));
}


void		QMyContact::emitClicked()
{
  emit		clicked();
}

void		QMyContact::oneContactNewConv()
{
  if (!_conv)
    bab->getChannelManager().CreatePrivateChannel(_id);
}

void		QMyContact::mouseDoubleClickEvent(QMouseEvent *ev)
{
  if (ev->button() == Qt::LeftButton)
    oneContactNewConv();
}

void		QMyContact::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    {
      if (event->modifiers() != Qt::ShiftModifier)
        _parent->clearList();
      _isSelected = true;
      setStyleSheet(guiStyle->selectContact());
      _parent->addToList(this);
    }
  else if (event->button() == Qt::RightButton)
  {
      QMenu	menu(this);
      QAction	newConv(this);
      QAction	callConv(this);
      QAction	rmContact(this);
      QAction	addToCurrentConversation(this);
      newConv.setText("New Conversation");
      callConv.setText("New Call Conversation");
      rmContact.setText("remove contact");
      addToCurrentConversation.setText("add this/these user(s) to the conversation");
      if(_parent->getList().empty())
      {
          newConv.setEnabled(false);
      }
      menu.addAction(&newConv);
      menu.addAction(&callConv);
      menu.addAction(&rmContact);
      menu.addAction(&addToCurrentConversation);
      if (_parent->getList().size() == 1)
        connect(&newConv, SIGNAL(triggered()), this, SLOT(oneContactNewConv()));
      else
        connect(&newConv, SIGNAL(triggered()), &bab->getChannelManager(), SLOT(newConversation()));
      connect(&rmContact, SIGNAL(triggered()), _parent, SLOT(rmContact()), Qt::QueuedConnection);
      connect(&addToCurrentConversation, SIGNAL(triggered()),
              &bab->getChannelManager(), SLOT(addContactInThisChannel()));
      menu.exec(QCursor::pos());
  }
  else
    _parent->clearList();
}

QMyContact::~QMyContact()
{
  delete ui;
  _parent = NULL;
}

bool		QMyContact::isSelected() const
{
  return _isSelected;
}

void		QMyContact::setIsSelected(bool isSelected)
{
  _isSelected = isSelected;
}

bool		QMyContact::operator==(QMyContact* contact)
{
  if (_id == contact->_id)
    return (true);
  return (false);
}

void  QMyContact::acceptClicked()
{
  ui->_accept->setHidden(true);
  ui->_refuse->setHidden(true);
  bab->getPacketCreator().Add_contact(this->_id);
}

void  QMyContact::setPending()
{
  ui->_accept->setHidden(false);
  ui->_refuse->setHidden(false);
}

void  QMyContact::refuseClicked()
{
  bab->getPacketCreator().Refuse_contact(this->_id);
  bab->getContactList()->deletePendingContact(this->_id);
}
