#include "configstylesheet.h"
#include "ui_configstylesheet.h"
#include <QApplication>
#include <QPalette>
#include <QDebug>
#include "rgbdial.h"
#include "babel.h"
#include "guistrstyle.h"

extern GuiStrStyle *guiStyle;

configStyleSheet::configStyleSheet(QWidget *parent) :
  QDialog(parent), _color(0, 0, 0),
  ui(new Ui::configStyleSheet)
{
  ui->setupUi(this);
  _rgbdial = new rgbDial(this);
  _role = QPalette::NoRole;
  _style = 0;
}

configStyleSheet::~configStyleSheet()
{
  delete ui;
}

void		configStyleSheet::changeGui(QString& vec, int const pos,
                                 QMap<char, int> const& map, fnptr ptr)
{
  QString	tmp;

  vec.remove(pos, 6);
  tmp = (map['r'] < 16 ? "0" : "") + QString::number(map['r'], 16);
  tmp += (map['g'] < 16 ? "0" : "") + QString::number(map['g'], 16);
  tmp += (map['b'] < 16 ? "0" : "") + QString::number(map['b'], 16);
  vec.insert(pos, tmp);
  (guiStyle->*ptr)(vec);
  _color.setRgb(map['r'], map['g'], map['b']);
}

void				configStyleSheet::changeStyle(QMap<char, int>& map, char type, int value)
{
  static QVector<QString>	vec;
  static QVector<int>		pos(9);

  if (vec.empty())
    {
      QString			tmp;

      vec.resize(9);
      pos[0] = 30;
      pos[1] = 46;
      pos[2] = 26;
      pos[3] = 17;
      pos[4] = 27;
      pos[5] = 28;
      pos[6] = 30;
      pos[7] = 53;
      pos[8] = 26;
      vec[2] = guiStyle->contact();
      vec[3] = guiStyle->contactText();
      vec[4] = guiStyle->profileBg();
      vec[5] = guiStyle->profileInfo();
      vec[6] = guiStyle->profileBox();
    }
  vec[0] = guiStyle->lineEdit();
  vec[1] = guiStyle->lineEdit();
  vec[7] = guiStyle->selectContact();
  vec[8] = guiStyle->selectContact();
  _color.getRgb(&map['r'], &map['g'], &map['b']);
  map[type] = value;
  static QVector<fnptr> vptr(9);
  vptr[0] = &GuiStrStyle::setLineEdit;
  vptr[1] = &GuiStrStyle::setLineEdit;
  vptr[2] = &GuiStrStyle::setContact;
  vptr[3] = &GuiStrStyle::setContactText;
  vptr[4] = &GuiStrStyle::setProfileBg;
  vptr[5] = &GuiStrStyle::setProfileInfo;
  vptr[6] = &GuiStrStyle::setProfileBox;
  vptr[7] = &GuiStrStyle::setSelectContact;
  vptr[8] = &GuiStrStyle::setSelectContact;
  changeGui(vec[_style - 1], pos[_style - 1], map,
        vptr[_style - 1]);
}

void configStyleSheet::onValueChanged(char type, int value)
{
  static QMap<char, int> map;
  map['r'] = 0;
  map['g'] = 0;
  map['b'] = 0;
  if (_style)
    {
      changeStyle(map, type, value);
      return ;
    }
  QColor color = QApplication::palette().color(_role);
  color.getRgb(&map['r'], &map['g'], &map['b']);
  map[type] = value;
  color.setRgb(map['r'], map['g'], map['b']);
  QPalette test = QApplication::palette();
  test.setColor(_role, color);
  QApplication::setPalette(test);
  bab->setPalette(test);

  QList<QWidget*> widgets = bab->findChildren<QWidget*>();
  foreach (QWidget* w, widgets)
    {
      w->setPalette(test);
    }
}

void configStyleSheet::on__windowBut_clicked()
{
  _style = 0;
  _role = QPalette::Window;
  _rgbdial->setValues(QApplication::palette().color(_role));
  _rgbdial->exec();
}

void configStyleSheet::on__windowTextBut_clicked()
{
  _style = 0;
  _role = QPalette::WindowText;
  _rgbdial->setValues(QApplication::palette().color(_role));
  _rgbdial->exec();
}

void configStyleSheet::on__baseBut_clicked()
{
  _style = 0;
  _role = QPalette::Base;
  _rgbdial->setValues(QApplication::palette().color(_role));
  _rgbdial->exec();
}

void configStyleSheet::on__alternateBaseBut_clicked()
{
  _style = 0;
  _role = QPalette::AlternateBase;
  _rgbdial->setValues(QApplication::palette().color(_role));
  _rgbdial->exec();
}

void configStyleSheet::on__toolTipBaseBut_clicked()
{
  _style = 0;
  _role = QPalette::ToolTipBase;
  _rgbdial->setValues(QApplication::palette().color(_role));
  _rgbdial->exec();
}

void configStyleSheet::on__toolTipText_clicked()
{
  _style = 0;
  _role = QPalette::ToolTipText;
  _rgbdial->setValues(QApplication::palette().color(_role));
  _rgbdial->exec();
}

void configStyleSheet::on__text_clicked()
{
  _style = 0;
  _role = QPalette::Text;
  _rgbdial->setValues(QApplication::palette().color(_role));
  _rgbdial->exec();
}

void configStyleSheet::on__buttonBut_clicked()
{
  _style = 0;
  _role = QPalette::Button;
  _rgbdial->setValues(QApplication::palette().color(_role));
  _rgbdial->exec();
}

void configStyleSheet::on__butonTextBut_clicked()
{
  _style = 0;
  _role = QPalette::ButtonText;
  _rgbdial->setValues(QApplication::palette().color(_role));
  _rgbdial->exec();
}

void configStyleSheet::on__brightTextBut_clicked()
{
  _style = 0;
  _role = QPalette::BrightText;
  _rgbdial->setValues(QApplication::palette().color(_role));
  _rgbdial->exec();
}

void configStyleSheet::on__linkBut_clicked()
{
  _style = 0;
  _role = QPalette::Link;
  _rgbdial->setValues(QApplication::palette().color(_role));
  _rgbdial->exec();
}

void configStyleSheet::on__hightlightBut_clicked()
{
  _style = 0;
  _role = QPalette::Highlight;
  _rgbdial->setValues(QApplication::palette().color(_role));
  _rgbdial->exec();
}

void configStyleSheet::on__hightlitedTextBut_clicked()
{
  _style = 0;
  _role = QPalette::HighlightedText;
  _rgbdial->setValues(QApplication::palette().color(_role));
  _rgbdial->exec();
}

void configStyleSheet::on__editLine_clicked()
{
  _style = 1;
  _color = guiStyle->getEditLineColor();
  _rgbdial->setValues(_color);
  _rgbdial->exec();
}

void configStyleSheet::on__editLineText_clicked()
{
  _style = 2;
  _color = guiStyle->getEditLineTextColor();
  _rgbdial->setValues(_color);
  _rgbdial->exec();
}


void configStyleSheet::on__contact_clicked()
{
  _style = 3;
  _color = guiStyle->getContactColor();
  _rgbdial->setValues(_color);
  _rgbdial->exec();
}

void configStyleSheet::on__contactText_clicked()
{
  _style = 4;
  _color = guiStyle->getContactTextColor();
  _rgbdial->setValues(_color);
  _rgbdial->exec();
}

void configStyleSheet::on__profileBg_clicked()
{
  _style = 5;
  _color = guiStyle->getProfileBgColor();
  _rgbdial->setValues(_color);
  _rgbdial->exec();
}

void configStyleSheet::on__profileInfo_clicked()
{
  _style = 6;
  _color = guiStyle->getProfileInfoColor();
  _rgbdial->setValues(_color);
  _rgbdial->exec();
}

void configStyleSheet::on__profileBox_clicked()
{
  _style = 7;
  _color = guiStyle->getProfileBoxColor();
  _rgbdial->setValues(_color);
  _rgbdial->exec();
}

void configStyleSheet::on__selectedContact_clicked()
{
  _style = 8;
  _color = guiStyle->getSelectedContactColor();
  _rgbdial->setValues(_color);
  _rgbdial->exec();
}

void configStyleSheet::on__selectedContactBorder_clicked()
{
  _style = 9;
  _color = guiStyle->getSelectedContactBorderColor();
  _rgbdial->setValues(_color);
  _rgbdial->exec();
}
