#ifndef CLICKLABEL_H
#define CLICKLABEL_H

#include <QLabel>

class ClickLabel : public QLabel
{
  Q_OBJECT

public:
  ClickLabel(QWidget *parent = 0, Qt::WindowFlags f = 0);

signals:
  void  clicked();
  void  isClicked();

public slots:
  void  mousePressEvent(QMouseEvent *);
};

#endif // CLICKLABEL_H
