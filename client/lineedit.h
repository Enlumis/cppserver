#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>

class LineEdit : public QLineEdit
{
  Q_OBJECT

public:
  LineEdit(QString str = "", QWidget* parent = 0);
  virtual   ~LineEdit();

signals:
  void      imFoccus(LineEdit*);
public slots:
  void      focusInEvent(QFocusEvent *focus);
  void      focusOutEvent(QFocusEvent *focus);
};

#endif // LINEEDIT_H
