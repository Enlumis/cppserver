#include "profile.h"
#include "ui_profile.h"
#include "guistrstyle.h"
#include <QDebug>

extern GuiStrStyle  *guiStyle;

Profile::Profile(QWidget *parent) :
  QFrame(parent),
  ui(new Ui::Profile)
{
  ui->setupUi(this);
//  this->setObjectName("test");
//  setStyleSheet("QGridLayout {border: 1px solid black; background-color: grey;}");
  ui->_comboBox_2->addItem(QPixmap(":/images/circle-green.png"), QString(""));
  ui->_comboBox_2->addItem(QPixmap(":/images/circle-red.png"), QString(""));
  ui->_comboBox_2->addItem(QPixmap(":/images/circle-yellow.png"), QString(""));
  ui->_comboBox_2->addItem(QPixmap(":/images/circle-grey.png"), QString(""));
  setStyleSheet(guiStyle->profileBg() + guiStyle->profileInfo() + guiStyle->profileBox());
  connect(guiStyle, SIGNAL(profileChanged()), this, SLOT(onProfileChanged()));
}

void	Profile::setPseudo(QString const& str)
{
  ui->_pseudo->setText(str);
}

QString	Profile::getPseudo() const
{
  return ui->_pseudo->text();
}

void	Profile::setStatus(QString const& str)
{
  ui->_email->setText(str);
}

QString Profile::getStatus() const
{
  return ui->_email->text();
}

Profile::~Profile()
{
  delete ui;
}

void	Profile::onProfileChanged()
{
  qDebug() << "PROFILE CHANGED !";
  setStyleSheet(guiStyle->profileBg() + guiStyle->profileInfo() + guiStyle->profileBox());
}

quint32	Profile::getId() const
{
  return _id;
}

void	Profile::setId(const quint32 &id)
{
  _id = id;
}
