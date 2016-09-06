#ifndef MYQDOCKWIDGET_H
#define MYQDOCKWIDGET_H

#include <QDockWidget>

class Convers;

class MyQDockWidget : public QDockWidget
{
  Q_OBJECT

  Convers*    _convers;
public:
    MyQDockWidget(QWidget * parent = 0, const QString & title = "", Qt::WindowFlags flags = 0);
    ~MyQDockWidget() {}
    void        setConvers(Convers* convers);
    Convers*    getConvers() const;

signals:
    void          clicked();
protected:
    virtual void  closeEvent(QCloseEvent * event);
public slots:
    void          onClicked();
    void          focusInEvent(QFocusEvent *);
};

#endif // MYQDOCKWIDGET_H
