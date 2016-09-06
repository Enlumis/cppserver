#ifndef CONTACTNAME_H
#define CONTACTNAME_H

#include <QLineEdit>

class QMyContact;

class ContactName : public QLineEdit
{
  QMyContact *_parent;
public:
  ContactName(QWidget *parent = 0);
signals:
  void clicked();
public slots:
  void  focusInEvent(QFocusEvent *);
};

#endif // CONTACTNAME_H
