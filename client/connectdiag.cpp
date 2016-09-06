#include "connectdiag.h"
#include "ui_connectdiag.h"
#include "babel.h"
#include "tcpsocket.h"
#include <QGridLayout>
#include <QDebug>
#include <QPushButton>
#include <QCheckBox>
#include <QCloseEvent>
#include <exception>

ConnectDiag::ConnectDiag(QWidget *parent) :
    QDialog(parent) , _salt("")
{
  ui = new Ui::ConnectDiag;
  ui->setupUi(this);
  ui->_errLab->setHidden(true);
  ui->_ip->setHidden(true);
  ui->_port->setHidden(true);
  ui->_ipLab->setHidden(true);
  ui->_portLab->setHidden(true);
  adjustSize();
  ui->_ip->setText("10.41.177.46");
  ui->_port->setText("4242");
  ui->_ip->setValidator(new QRegExpValidator(QRegExp("(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"
                                                   "\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)"), this));
  ui->_port->setValidator(new QIntValidator(0, 65535, ui->_port));

  _ssize = size();
}

void    ConnectDiag::on__adopt_stateChanged(int val)
{
    if (val)
    {
        ui->_ip->setHidden(false);
        ui->_port->setHidden(false);
        ui->_ipLab->setHidden(false);
        ui->_portLab->setHidden(false);
    }
    else
    {
        ui->_ip->setHidden(true);
        ui->_port->setHidden(true);
        ui->_ipLab->setHidden(true);
        ui->_portLab->setHidden(true);
        adjustSize();
    }
    show();
}

void    ConnectDiag::on__buttons_rejected()
{
//    qDebug() << "cancel !";
}

void    ConnectDiag::setIpPortErrorLab(const QString &errmsg)
{
  ui->_errLab->setText(errmsg);
  ui->_errLab->setStyleSheet("QLabel {color: red;}");
  ui->_errLab->setHidden(false);
}

void    ConnectDiag::setIpPortSuccesLab(const QString &msg)
{
  ui->_errLab->setText(msg);
  ui->_errLab->setStyleSheet("QLabel {color: green;}");
  ui->_errLab->setHidden(false);
}

void        ConnectDiag::setIpErrorStylesheet()
{
//  _ip->setStyleSheet("QLineEdit{border: 1px solid rgb(255, 0, 0);}");
}

void        ConnectDiag::setPortErrorStylesheet()
{
//  _port->setStyleSheet("QLineEdit{border: 1px solid rgb(255, 0, 0);}");
}

void ConnectDiag::on__buttons_accepted()
{
//  qDebug() << "NOOOOOO !";
}

void ConnectDiag::tryToConnect()
{
  qDebug() << "TSocket =" << bab->getTSocket();
//  if (bab->getTSocket()->ConnectToServer(ui->_ip->text(), QVariant(ui->_port->text()).toUInt()))
//    emit this->accept();
}

void ConnectDiag::on_buttonBox_accepted()
{
  try {
    if (!bab->hasSocket())
      {
        qDebug() << "[Debug] Test connection : {" << ui->_ip->text() << ":" << ui->_port->text() << "}";
        bab->setTsocket(new TcpSocket(ui->_ip->text(), QVariant(ui->_port->text()).toUInt(), this));
        qDebug() << "[Debug] Connection : Success.";
      }
    else
      {
        if (ui->_register->isChecked())
          {
            bab->getPacketCreator().Register(ui->_login->text().toStdString(), ui->_pass->text().toStdString());
          }
        else if (_salt != "")
          {
            bab->getPacketCreator().Authenticate(_salt, ui->_login->text().toStdString(), ui->_pass->text().toStdString());
          }
      }
  } catch (std::exception&) {
    qDebug() << "[Debug] Connection : Failure.";
  }
}

void ConnectDiag::on_buttonBox_rejected()
{
  this->done(-1);
}

void             ConnectDiag::setUsername( QString& username )
{
  ui->_login->setText(username);
}

void             ConnectDiag::setPassword( QString& password )
{
  ui->_pass->setText(password);
}

QString   ConnectDiag::getUsername() const
{
  return ui->_login->text();
}

QString   ConnectDiag::getPassword() const
{
  return ui->_pass->text();
}

bool      ConnectDiag::isRegisterChecked() const
{
  return ui->_register->isChecked();
}

void      ConnectDiag::notify(bool isError, std::string const& msg)
{
  if (isError)
    ui->_errLab->setStyleSheet("QLabel{color: red;}");
  else
    ui->_errLab->setStyleSheet("QLabel{color: green;}");
  ui->_errLab->setText((isError == true ? "Error: " : "Success: ") + QString::fromStdString(msg));
  ui->_errLab->setHidden(false);
}

void      ConnectDiag::closeEvent(QCloseEvent *ev)
{
  ui->_errLab->setHidden(true);
  finished(-1);
  QDialog::closeEvent(ev);
}

std::string const&      ConnectDiag::getSalt() const
{
  return _salt;
}

void                    ConnectDiag::setSalt(std::string const& salt)
{
  _salt = salt;
}

void                    ConnectDiag::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Escape)
    return ;
  else
    QDialog::keyPressEvent(event);
}
