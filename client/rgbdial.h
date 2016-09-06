#ifndef RGBDIAL_H
#define RGBDIAL_H

#include <QDialog>

namespace Ui {
  class rgbDial;
}

class rgbDial : public QDialog
{
  Q_OBJECT

public:
  explicit rgbDial(QWidget *parent = 0);
  ~rgbDial();
  void  setValues(QColor color);

private slots:
  void on__blue_valueChanged(int value);

  void on__green_valueChanged(int value);

  void on__red_valueChanged(int value);

  void on__redVal_returnPressed();

  void on__greenVal_returnPressed();

  void on__blueVal_returnPressed();

private:
  Ui::rgbDial *ui;
};

#endif // RGBDIAL_H
