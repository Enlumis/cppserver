#include "guistrstyle.h"
#include <QColor>
#include <QDebug>
#include <QRegExp>
#include <QApplication>

extern QApplication *app;

GuiStrStyle::GuiStrStyle()
{
  _lineEdit = QString("QLineEdit {background-color: #000000; color: #ffffff;}");
  _contactText = QString("#_login {color: #ffffff;}");
  _contact = QString("#test {background-color: #000000; border: 1px; margin: 2px; border-radius: 5px;}");
  _selectContact = QString("#test {border: 1px solid #000000; background-color: #a0a0a4;}");
  _profilebg = QString("#frame {background-color: #000000; border: 1px; border-radius: 5px;}");
  _profileInfo = QString("#_frame {background-color: #a0a0a4; border: 1px; border-radius: 5px;}");
  _profileBox = QString("QComboBox {background-color: #a0a0a4;}");
  connect(this, SIGNAL(lineEditChanged()), this, SLOT(onLineEditChanged()));
  connect(this, SIGNAL(contactChanged()), this, SLOT(onContactChanged()));
}

QColor		GuiStrStyle::getEditLineColor()
{
  QColor	color(0, 0, 0);
  QRegExp	rx("QLineEdit\? \\{background-color: #(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})");
  rx.indexIn(_lineEdit);
  bool		ok;
  color.setRgb(rx.cap(1).toUInt(&ok, 16), rx.cap(3).toUInt(&ok, 16), rx.cap(5).toUInt(&ok, 16));
  return (color);
}

QColor		GuiStrStyle::getEditLineTextColor()
{
  QColor	color(0, 0, 0);
  QRegExp	rx("QLineEdit\? \\{background-color: #(([0-9]|[a-f]){6}); color: #(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})");
  rx.indexIn(_lineEdit);
  bool		ok;
  color.setRgb(rx.cap(3).toUInt(&ok, 16), rx.cap(5).toUInt(&ok, 16), rx.cap(7).toUInt(&ok, 16));
  return (color);
}

QColor		GuiStrStyle::getSelectedContactColor()
{
  QColor	color(0, 0, 0);
  QRegExp	rx("#test \\{border: 1px solid #(([0-9]|[a-f]){6}); background-color: #(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})");
  rx.indexIn(_selectContact);
  bool		ok;
  color.setRgb(rx.cap(3).toUInt(&ok, 16), rx.cap(5).toUInt(&ok, 16), rx.cap(7).toUInt(&ok, 16));
  return (color);
}

QColor		GuiStrStyle::getSelectedContactBorderColor()
{
  QColor	color(0, 0, 0);
  QRegExp	rx("#test \\{border: 1px solid #(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})");
  rx.indexIn(_selectContact);
  bool		ok;
  color.setRgb(rx.cap(1).toUInt(&ok, 16), rx.cap(3).toUInt(&ok, 16), rx.cap(5).toUInt(&ok, 16));
  return (color);
}

QColor		GuiStrStyle::getContactColor()
{
  QColor	color(0, 0, 0);
  QRegExp	rx("#test? \\{background-color: #(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})");
  rx.indexIn(_contact);
  bool		ok;
  color.setRgb(rx.cap(1).toUInt(&ok, 16), rx.cap(3).toUInt(&ok, 16), rx.cap(5).toUInt(&ok, 16));
  return (color);
}

QColor		GuiStrStyle::getContactTextColor()
{
  QColor	color(0, 0, 0);
  QRegExp	rx("#_login? \\{color: #(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})");
  rx.indexIn(_contactText);
  bool		ok;
  color.setRgb(rx.cap(1).toUInt(&ok, 16), rx.cap(3).toUInt(&ok, 16), rx.cap(5).toUInt(&ok, 16));
  return (color);
}

QColor		GuiStrStyle::getProfileBgColor()
{
  QColor	color(0, 0, 0);
  QRegExp	rx("#frame? \\{background-color: #(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})");
  rx.indexIn(_profilebg);
  bool		ok;
  color.setRgb(rx.cap(1).toUInt(&ok, 16), rx.cap(3).toUInt(&ok, 16), rx.cap(5).toUInt(&ok, 16));
  return (color);
}

QColor		GuiStrStyle::getProfileInfoColor()
{
  QColor	color(0, 0, 0);
  QRegExp	rx("#_frame? \\{background-color: #(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})");
  rx.indexIn(_profileInfo);
  bool		ok;
  color.setRgb(rx.cap(1).toUInt(&ok, 16), rx.cap(3).toUInt(&ok, 16), rx.cap(5).toUInt(&ok, 16));
  return (color);
}

QColor		GuiStrStyle::getProfileBoxColor()
{
  QColor	color(0, 0, 0);
  QRegExp	rx("QComboBox \\{background-color: #(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})(([0-9]|[a-f]){2})");
  rx.indexIn(_profileBox);
  bool		ok;
  color.setRgb(rx.cap(1).toUInt(&ok, 16), rx.cap(3).toUInt(&ok, 16), rx.cap(5).toUInt(&ok, 16));
  return (color);
}

void		GuiStrStyle::onLineEditChanged()
{
  app->setStyleSheet(_lineEdit + _contact + _contactText);
}

void		GuiStrStyle::onContactChanged()
{
  app->setStyleSheet(_lineEdit + _contact + _contactText);
}

QString		GuiStrStyle::selectContact() const
{
  return _selectContact;
}

void		GuiStrStyle::setSelectContact(const QString &selectContact)
{
  _selectContact = selectContact;
  emit		selectContactChanged();
}

QString		GuiStrStyle::contact() const
{
  return _contact;
}

void		GuiStrStyle::setContact(const QString &contact)
{
  _contact = contact;
  emit		contactChanged();
}

QString		GuiStrStyle::contactText() const
{
  return _contactText;
}

void		GuiStrStyle::setContactText(const QString &contactText)
{
  _contactText = contactText;
  emit		contactChanged();
}

QString		GuiStrStyle::profileBg() const
{
  return _profilebg;
}

void		GuiStrStyle::setProfileBg(const QString &profile)
{
  _profilebg = profile;
  emit		profileChanged();
}

QString		GuiStrStyle::profileInfo() const
{
  return _profileInfo;
}

void		GuiStrStyle::setProfileInfo(const QString &profileInfo)
{
  _profileInfo = profileInfo;
  emit		profileChanged();
}

QString		GuiStrStyle::profileBox() const
{
  return _profileBox;
}

void		GuiStrStyle::setProfileBox(const QString &profileBox)
{
  _profileBox = profileBox;
  emit		profileChanged();
}

QString		GuiStrStyle::lineEdit() const
{
  return _lineEdit;
}

void		GuiStrStyle::setLineEdit(const QString &lineEdit)
{
  _lineEdit = lineEdit;
  emit		lineEditChanged();
}
