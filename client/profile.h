#ifndef PROFILE_H
#define PROFILE_H

#include <QWidget>
#include <QFrame>

namespace Ui {
  class Profile;
}

class Profile : public QFrame
{
  Q_OBJECT
  class ProfileLayout : public QFrame
  {
     QLayout *_lay;
  };

public:
  explicit Profile(QWidget *parent = 0);
//  Ui::Profile* getUi() {return ui;}
  void setPseudo(QString const& str);
  QString getPseudo() const;
  void setStatus(QString const& str);
  QString getStatus() const;
  ~Profile();

  quint32 getId() const;
  void setId(const quint32 &id);

public slots:
  void  onProfileChanged();

private:
  Ui::Profile *ui;
  quint32     _id;
};

#endif // PROFILE_H
