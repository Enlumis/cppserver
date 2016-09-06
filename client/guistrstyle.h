#ifndef GUISTRSTYLE_H
#define GUISTRSTYLE_H

#include <QString>
#include <QObject>
#include <QColor>

class GuiStrStyle : public QObject
{
  Q_OBJECT

  QString   _lineEdit;
  QString   _profilebg;
  QString   _profileInfo;
  QString   _profileBox;
  QString   _contactText;
  QString   _contact;
  QString   _selectContact;
public:
            GuiStrStyle();
  QString   selectContact() const;
  void      setSelectContact(const QString &selectContact);
  QString   contact() const;
  void      setContact(const QString &contact);
  QString   profileBg() const;
  void      setProfileBg(const QString &profile);
  QString   lineEdit() const;
  void      setLineEdit(const QString &lineEdit);
  QColor    getEditLineColor();
  QColor    getEditLineTextColor();
  QColor    getContactColor();
  QColor    getContactTextColor();
  QColor    getProfileBgColor();
  QColor    getProfileInfoColor();
  QColor    getProfileBoxColor();
  QColor    getSelectedContactColor();
  QColor    getSelectedContactBorderColor();
  QString   contactText() const;
  void      setContactText(const QString &contactText);

  QString   profileInfo() const;
  void      setProfileInfo(const QString &profileInfo);

  QString   profileBox() const;
  void      setProfileBox(const QString &profileBox);

signals:
  void      lineEditChanged();
  void      profileChanged();
  void      contactChanged();
  void      selectContactChanged();
private slots:
  void      onLineEditChanged();
  void      onContactChanged();
};

#endif // GUISTRSTYLE_H
