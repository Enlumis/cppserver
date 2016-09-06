#include "contactlist.h"
#include "qmycontact.h"
#include <QLineEdit>
#include <QEvent>
#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>
#include <QShortcut>
#include "babel.h"
#include "guistrstyle.h"
#include "../server/src/packets/PacketAddContact.hh"
#include "babel.h"
#include "ui_babel.h"
#include <QVector>

extern GuiStrStyle *guiStyle;

QVector<Type::UserField> testContactList()
{
  Type::UserField ci1;
  Type::UserField ci2;
  Type::UserField ci3;
  Type::UserField ci4;
  Type::UserField ci5;

  ci1.username = "Gabriel";
  ci1.status = "Pourquoi ?";
  ci1.uid = 42;
  ci2.username = "Valentin";
  ci2.status = "youpi";
  ci2.uid = 1;
  ci3.username = "Jerome";
  ci3.status = "cheveux court";
  ci3.uid = 2;
  ci4.username = "Kevin";
  ci4.status = "Designer";
  ci4.uid = 3;
  ci5.username = "Antoine";
  ci5.status = "A pas l'air de super m'apprecier";
  ci5.uid = 4;

  QVector<Type::UserField> vec;
  vec.resize(5);
  vec[0] = ci1;
  vec[1] = ci2;
  vec[2] = ci3;
  vec[3] = ci4;
  vec[4] = ci5;
  return vec;
}


ContactList::ContactList(QWidget *parent) :
    QWidget(parent), _selectlist(), _layout(new QVBoxLayout(this)), _errLab(new QLabel(this)), _contactList(0)
{
    _layout->setAlignment(Qt::AlignTop);
    _layout->setSpacing(0);
    _errLab->setHidden(true);
    _layout->addWidget(_errLab);
//    setContactList(testContactList());
//    addContact(new QMyContact("Hengrui", this));
    connect(new QShortcut(QKeySequence(Qt::Key_Delete), this), SIGNAL(activated()), this, SLOT(rmContact()));
    connect(new QShortcut(QKeySequence(Qt::Key_Down), this), SIGNAL(activated()), this, SLOT(downContact()));
    connect(new QShortcut(QKeySequence(Qt::Key_Up), this), SIGNAL(activated()), this, SLOT(UpContact()));
}

void      ContactList::updateContactUsername(quint32 id, std::string data)
{
  for (QVector<QMyContact*>::iterator it = _contactList.begin() ; it != _contactList.end() ; it++)
    {
      if ((*it)->getId() == id)
        {
          (*it)->setUserName(QString::fromStdString(data));
          break;
        }
    }
}

void      ContactList::updateContactStatus(quint32 id, std::string data)
{
  for (QVector<QMyContact*>::iterator it = _contactList.begin() ; it != _contactList.end() ; it++)
    {
      if ((*it)->getId() == id)
        {
          (*it)->setStatus(QString::fromStdString(data));
          (*it)->setToolTip(QString::fromStdString(data));
          break;
        }
    }
}


void      ContactList::addPendingContact(quint32 id, const std::string &userName, const std::string &status)
{
  QMyContact *contact = new QMyContact(QString::fromStdString(userName), this);
  contact->setId(id);
  contact->setStatus(QString::fromStdString(status));
  contact->setPending();
  addContact(contact);
}

void      ContactList::deletePendingContact(uint32_t userId)
{
  qDebug() << "userId = " << userId << " contactList.size = " << _contactList.size();
  for (int i = 0; i < _contactList.size() ; i++)
    {
      qDebug() << "id = " << _contactList[i]->getId();
      if (_contactList[i]->getId() == userId)
        {
          _layout->removeWidget(_contactList[i]);
          _contactList[i]->setHidden(true);
          _contactList[i]->setParent(NULL);
          QMyContact* test = _contactList[i];
          _contactList.remove(i);
          delete (test);
          break;
        }
    }
}

std::list<QMyContact*>&  ContactList::getList()
{
    return _selectlist;
}

std::list<QMyContact*>const&  ContactList::getList() const
{
    return _selectlist;
}

QVector<QMyContact*> const& ContactList::getContactList() const
{
  return _contactList;
}

void        ContactList::clearContactList()
{
  for (QVector<QMyContact*>::iterator it = _contactList.begin() ; it != _contactList.end() ; it++)
    {
      _layout->removeWidget(*it);
      (*it)->setHidden(true);
    }
  _contactList.clear();
}

void        ContactList::setContactList(QVector<Type::UserField> const& vec)
{
  clearContactList();
  _contactList.resize(vec.size());
  for (int i = 0 ; i < vec.size() ; i++)
    {
      _contactList[i] = new QMyContact(QString(""), this);
      _contactList[i]->setUserName(QString::fromStdString(vec[i].username));
      _contactList[i]->setId(vec[i].uid);
      _contactList[i]->setStatus(QString::fromStdString(vec[i].status));
      _contactList[i]->setToolTip(QString::fromStdString(vec[i].status));
      qDebug() << "[Debug] setContactList: username = "
               << _contactList[i]->getUserName() << " uid = " << _contactList[i]->getId();
      _layout->addWidget(_contactList[i]);
    }
}

void      ContactList::addContact(QString const& userName, QString const& status, quint32 id)
{
  QMyContact* newContact = new QMyContact(userName, this);

  newContact->setStatus(status);
  newContact->setId(id);
  addContact(newContact);
}

void      ContactList::addContact(std::string const& userName, std::string const& status, uint32_t id)
{
  addContact(QString::fromStdString(userName), QString::fromStdString(status), id);
}

bool      ContactList::isAlreadyInContactList(QMyContact* contact)
{
  for (QVector<QMyContact*>::iterator it = _contactList.begin() ; it != _contactList.end() ; it++)
    {
      qDebug() << "contact userName: " << contact->getUserName() << " id: " << contact->getId();
      qDebug() << "it userName: " << (*it)->getUserName() << " id : " << (*it)->getId();
      if (*contact == (*it))
        return (true);
    }
  return (false);
}

void      ContactList::addContact(QMyContact* newContact)
{
  if (isAlreadyInContactList(newContact))
    {
      newContact->setParent(NULL);
      delete newContact;
      return ;
    }
  newContact->setToolTip(newContact->getStatus());
  _contactList.push_back(newContact);
  _layout->addWidget(newContact);
  qDebug() << "add Contact: contact added !";
}

//void    ContactList::setContactList()
//{
//    QMyContact *line = new QMyContact("Gabriel", this);
//    QMyContact *line2 = new QMyContact("Jerôme", this);
//    QMyContact *line3 = new QMyContact("Valentin", this);
//    QMyContact *line4 = new QMyContact("Antoine", this);
//    QMyContact *line5 = new QMyContact("Kévin", this);
//    _layout->addWidget(line);
//    _layout->addWidget(line2);
//    _layout->addWidget(line3);
//    _layout->addWidget(line4);
//    _layout->addWidget(line5);
//}

void        ContactList::clearList()
{
  for (std::list<QMyContact*>::iterator it = _selectlist.begin()
       ; it != _selectlist.end() ; it++)
    {
      (*it)->setStyleSheet("");
      (*it)->setIsSelected(false);
    }
  _selectlist.clear();
}

void        ContactList::removeList()
{
  while (!_selectlist.empty())
  {
    std::list<QMyContact*>::iterator it = _selectlist.begin();
    _selectlist.remove(*it);
    (*it)->setParent(NULL);
    (*it)->setHidden(true);
    (*it)->setIsSelected(false);
    delete (*it);
    }
}

void        ContactList::addToList(QMyContact *contact)
{
    _selectlist.push_back(contact);
}

void        ContactList::removeContact(QMyContact* contact)
{
  for (int i = 0; i < _contactList.size() ; i++)
    {
      if (_contactList[i] == contact)
        {
          _contactList.remove(i);
          _layout->removeWidget(contact);
          break;
        }
    }
}

void       ContactList::rmContact()
{
  QMessageBox::StandardButton reply;
  reply = QMessageBox::question(this, "Remove contact(s)", "Are you sure you want remove this/these contact(s) from your contact list ?",
                                QMessageBox::Yes|QMessageBox::No);
  if (reply != QMessageBox::Yes)
    return ;
  for (std::list<QMyContact*>::iterator it = _selectlist.begin() ; it != _selectlist.end() ; it++)
    {
      _layout->removeWidget(*it);
    }
  removeList();
}

QMyContact*   ContactList::PopLastItem()
{
  if (_selectlist.empty())
    return NULL;
  QMyContact* tmp = _selectlist.back();
  _selectlist.pop_back();
  tmp->setStyleSheet("");
  tmp->setIsSelected(false);
  return tmp;
}

void      ContactList::addNextItem(QMyContact *topush)
{
  topush->setStyleSheet(guiStyle->selectContact());
  topush->setIsSelected(true);
  _selectlist.push_back(topush);
}

void      ContactList::downContact()
{
  QMyContact* tmp;
  if (!(tmp = ContactList::PopLastItem()))
    return ;
  QMyContact* topush = static_cast<QMyContact*>(_layout->itemAt((_layout->indexOf(tmp) + 1) % _layout->count())->widget());
  addNextItem(topush);
}

void      ContactList::UpContact()
{
  QMyContact* tmp;
  if (!(tmp = ContactList::PopLastItem()))
    return ;
  int save = (_layout->indexOf(tmp) == 0 ? _layout->count() - 1 : _layout->indexOf(tmp) - 1);
  QMyContact* topush = static_cast<QMyContact*>(_layout->itemAt(save % _layout->count())->widget());
  addNextItem(topush);
}

QString             ContactList::getUsernameToId(uint32_t id) const
{
  for (QVector<QMyContact*>::const_iterator it = _contactList.begin()
       ; it != _contactList.end() ; it++)
    {
      if ((*it)->getId() == id)
        return ((*it)->getUserName());
    }
  return ("");
}

void      ContactList::notify(bool isError, const std::string &msg)
{
  if (isError)
    _errLab->setStyleSheet("QLabel{color: red;}");
  else
    _errLab->setStyleSheet("QLabel{color: green;}");
  _errLab->setHidden(false);
  _errLab->setText((isError == true ? "Error: " : "Success: ") + QString::fromStdString(msg));
  QTimer::singleShot(5*1000, this, SLOT(endErrorMessage()));
}

void    ContactList::endErrorMessage()
{
  _errLab->setHidden(true);
}

const std::list<QMyContact *> &ContactList::getSelectedList() const
{
  return _selectlist;
}

