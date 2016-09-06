#include "clicklabel.h"
#include <QDebug>

ClickLabel::ClickLabel(QWidget *parent, Qt::WindowFlags f)
  : QLabel(parent, f)
{
}

void	ClickLabel::mousePressEvent(QMouseEvent *)
{
  qDebug() << "click label pressed !";
  emit	isClicked();
}
