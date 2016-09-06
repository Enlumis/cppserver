#ifndef QMYCONTACT_H
#define QMYCONTACT_H

#include <QFrame>
#include <QTimer>
#include "contactlist.h"

namespace Ui {
  class QMyContact;
}

class MyQDockWidget;

class QMyContact : public QFrame
{
  Q_OBJECT

  quint32               _id;
  bool                  _isSelected;
  MyQDockWidget         *_conv;
  ContactList           *_parent;
  QString               _status;

public:
  explicit              QMyContact(const QString &str, QWidget *parent = 0);
  ~QMyContact();
  Ui::QMyContact*       getUi() {return ui;}
  void                  emitClicked();
  void                  mousePressEvent(QMouseEvent *);

  bool                  isSelected() const;
  void                  setIsSelected(bool isSelected);
  void                  setUserName(QString const& username);
  QString               getUserName() const;
  void                  setStatus(QString const& status);
  QString const&        getStatus() const;
  void                  setId(quint32 id);
  quint32               getId() const;
  bool                  operator==(QMyContact* contact);
  void                  setPending();

signals:
  void                  clicked();

private slots:
  void                  isAfk();
  void                  isConnected();
  void                  isDisconnected();
  void                  mouseDoubleClickEvent(QMouseEvent *ev);
  void                  oneContactNewConv();
  void                  onSelectContactChanged();
  void                  acceptClicked();
  void                  refuseClicked();


private:
  Ui::QMyContact        *ui;

};

#endif // QMYCONTACT_H
