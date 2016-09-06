#include "addcontact.h"
#include "ui_addcontact.h"
#include "lineedit.h"
#include "babel.h"
#include "contactlist.h"
#include "../server/src/packets/PacketSearchUser.hh"
#include <QDebug>

AddContact::AddContact(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::AddContact)
{
  ui->setupUi(this);
  ui->_errLab->setHidden(true);
}

AddContact::~AddContact()
{
  delete ui;
}

void			AddContact::on__searchButton_clicked()
{
  PacketSearchUser	*packet = new PacketSearchUser;
  packet->setPattern(ui->_searchContact->text().toStdString());
  bab->getTSocket()->send(packet);
}

void			AddContact::setSearchResultList(const std::vector<PacketSearchResult::user_t> &foundContactList)
{
  _foundContactList = foundContactList;
  while (ui->_findContactListLayout->count())
    {
      QWidget		*w = ui->_findContactListLayout->itemAt(0)->widget();
      ui->_findContactListLayout->removeWidget(w);
      delete w;
    }
  for (size_t i = 0 ; i < foundContactList.size() ; i++)
    {
      LineEdit		*contactFound = new LineEdit;
      connect(contactFound, SIGNAL(imFoccus(LineEdit*)), this, SLOT(onLineEditFocus(LineEdit*)));
      contactFound->setReadOnly(true);
      contactFound->setText(QString::fromStdString(foundContactList[i].userName));
      ui->_findContactListLayout->addWidget(contactFound);
    }
}

void			AddContact::onLineEditFocus(LineEdit* contactFocus)
{
  _contactFocus = contactFocus;
}

void			AddContact::on__addUser_clicked()
{
  qDebug() << "[Debug] Add Contact" << _contactFocus->text();
  for (size_t i = 0 ; i < _foundContactList.size() ; i++)
    {

      if (_foundContactList[i].userName == _contactFocus->text().toStdString())
        {
          qDebug() << "[Debug] Add Contact";
          bab->getPacketCreator().Add_contact(_foundContactList[i].userId);

          break;
        }
    }
}

void			AddContact::notify(bool isError, std::string const& msg)
{
  if (isError)
    ui->_errLab->setStyleSheet("QLabel{color: red;}");
  else
    ui->_errLab->setStyleSheet("QLabel{color: green;}");
  ui->_errLab->setHidden(false);
  ui->_errLab->setText(QString((isError == true ? "Error: " : "Success: ")) + QString::fromStdString(msg));
}

void			AddContact::closeEvent(QCloseEvent *ev)
{
  ui->_errLab->setHidden(true);
  QDialog::closeEvent(ev);
}
