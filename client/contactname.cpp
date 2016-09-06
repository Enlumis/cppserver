#include "contactname.h"
#include <QDebug>
#include <QMouseEvent>
#include "qmycontact.h"

ContactName::ContactName(QWidget *parent)
  : QLineEdit(parent), _parent(static_cast<QMyContact*>(parent))
{
}

void  ContactName::focusInEvent(QFocusEvent *)
{
  _parent->mousePressEvent(new QMouseEvent(QEvent::MouseButtonPress,
                                           pos(), Qt::LeftButton,
                                           Qt::NoButton, Qt::NoModifier));
}
