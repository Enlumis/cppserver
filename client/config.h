#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>

namespace Ui {
  class Config;
}

class configStyleSheet;

class Config : public QDialog
{
  Q_OBJECT


public:
  explicit    Config(QWidget *parent = 0);
              ~Config();
  void        notify(bool isError, const std::string &msg);

private slots:
  void        on_buttonBox_accepted();
  void        on__changeUsername_returnPressed();
  void        on__changeStatus_returnPressed();
  void        closeEvent(QCloseEvent *);

private:
  Ui::Config  *ui;
};

#endif // CONFIG_H
