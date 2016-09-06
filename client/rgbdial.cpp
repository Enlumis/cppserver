#include "rgbdial.h"
#include "ui_rgbdial.h"
#include "configstylesheet.h"
#include <QDebug>

rgbDial::rgbDial(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::rgbDial)
{
  ui->setupUi(this);
}

rgbDial::~rgbDial()
{
  delete ui;
}

void	rgbDial::setValues(QColor color)
{
  int	r, g, b;

  color.getRgb(&r, &g, &b);
  ui->_red->setValue(r);
  ui->_green->setValue(g);
  ui->_blue->setValue(b);
  ui->_redVal->setText(QVariant(r).toString());
  ui->_greenVal->setText(QVariant(g).toString());
  ui->_blueVal->setText(QVariant(b).toString());
  ui->_redVal->setValidator(new QIntValidator(0, 255));
  ui->_greenVal->setValidator(new QIntValidator(0, 255));
  ui->_blueVal->setValidator(new QIntValidator(0, 255));
}

void	rgbDial::on__green_valueChanged(int value)
{
  ui->_greenVal->setText(QVariant(value).toString());
  static_cast<configStyleSheet*>(parent())->onValueChanged('g', value);
}

void	rgbDial::on__blue_valueChanged(int value)
{
    ui->_blueVal->setText(QVariant(value).toString());
    static_cast<configStyleSheet*>(parent())->onValueChanged('b', value);
}

void	rgbDial::on__red_valueChanged(int value)
{
  ui->_redVal->setText(QVariant(value).toString());
  static_cast<configStyleSheet*>(parent())->onValueChanged('r', value);
}

void	rgbDial::on__redVal_returnPressed()
{
  ui->_red->setValue(QVariant(ui->_redVal->text()).toInt());
  static_cast<configStyleSheet*>(parent())->onValueChanged('r', ui->_red->value());
}

void	rgbDial::on__greenVal_returnPressed()
{
  ui->_green->setValue(QVariant(ui->_greenVal->text()).toInt());
  static_cast<configStyleSheet*>(parent())->onValueChanged('g', ui->_green->value());
}

void	rgbDial::on__blueVal_returnPressed()
{
  ui->_blue->setValue(QVariant(ui->_blueVal->text()).toInt());
  static_cast<configStyleSheet*>(parent())->onValueChanged('b', ui->_blue->value());
}
