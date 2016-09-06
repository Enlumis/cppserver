#include "config.h"
#include "ui_config.h"
#include <QDebug>
#include <QVariant>
#include "babel.h"
#include "ui_babel.h"
#include "contactlist.h"
#include "configstylesheet.h"
#include "PacketUpdateUsername.hh"
#include "PacketUpdateStatus.hh"

Config::Config(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Config)
{
  ui->setupUi(this);
  ui->_errLab->setHidden(true);
}

Config::~Config()
{
  delete ui;
}

void			Config::on_buttonBox_accepted()
{
}

void			Config::on__changeUsername_returnPressed()
{
  PacketUpdateUsername	*packet = new PacketUpdateUsername;

  packet->setUsername(ui->_changeUsername->text().toStdString());
  bab->getTSocket()->send(packet);
  delete packet;
}

void			Config::on__changeStatus_returnPressed()
{
  PacketUpdateStatus	*packet = new PacketUpdateStatus;

  packet->setStatus(ui->_changeStatus->text().toStdString());
  bab->getTSocket()->send(packet);
  delete packet;
}

void			Config::notify(bool isError, std::string const& msg)
{
  if (isError)
    {
      ui->_errLab->setStyleSheet("QLabel{color: red;}");
    }
  else
    ui->_errLab->setStyleSheet("QLabel{color: green;}");
  ui->_errLab->setHidden(false);
  ui->_errLab->setText((isError == true ? "Error: " : "Success: ") + QString::fromStdString(msg));
}

void		      Config::closeEvent(QCloseEvent *ev)
{
  ui->_errLab->setHidden(true);
  QDialog::closeEvent(ev);
}


