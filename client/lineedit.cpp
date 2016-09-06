#include "lineedit.h"
#include <QDebug>

LineEdit::LineEdit(QString str, QWidget* parent)
  : QLineEdit(str, parent)
{
}

LineEdit::~LineEdit()
{}

void    LineEdit::focusInEvent(QFocusEvent *focus)
{
  QLineEdit::focusInEvent(focus);
  emit	imFoccus(this);
}

void    LineEdit::focusOutEvent(QFocusEvent *focus)
{
  QLineEdit::focusOutEvent(focus);
}
