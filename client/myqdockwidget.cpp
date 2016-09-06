#include "myqdockwidget.h"
#include <QDebug>
#include <QCloseEvent>
#include "babel.h"
#include "ui_babel.h"
#include "convers.h"

extern Babel *bab;

MyQDockWidget::MyQDockWidget(QWidget * parent, const QString & title, Qt::WindowFlags flags)
    : QDockWidget(title, parent, flags)
{
  setAllowedAreas(Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
  setFocusPolicy(Qt::StrongFocus);
}

void	MyQDockWidget::onClicked()
{
  qDebug() << "je suis cliqué !";
}

void	MyQDockWidget::closeEvent(QCloseEvent *event)
{
    if (isFloating() == false)
        return ;
    setFloating(false);
    event->ignore();
}

void	MyQDockWidget::setConvers(Convers* convers)
{
  _convers = convers;
  setWidget(convers);
}

Convers* MyQDockWidget::getConvers() const
{
  return _convers;
}

void     MyQDockWidget::focusInEvent(QFocusEvent *focus)
{
  qDebug() << "[MyQDockWidget]: is focus !";
  QDockWidget::focusInEvent(focus);
}
