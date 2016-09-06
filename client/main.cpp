#include	"babel.h"
#include	<QApplication>
#include	<QStyleFactory>
#include	<QLabel>
#include	"guistrstyle.h"
#include	<stdexcept>

Babel		*bab = NULL;
GuiStrStyle	*guiStyle = NULL;
QApplication	*app = NULL;

void		darkStyle(QPalette& darkPalette)
{
  darkPalette.setColor(QPalette::Window, QColor(53,53,53));
  darkPalette.setColor(QPalette::WindowText, QColor(255, 255, 255));
  darkPalette.setColor(QPalette::Base, QColor(25,25,25));
  darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
  darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
  darkPalette.setColor(QPalette::ToolTipText, Qt::white);
  darkPalette.setColor(QPalette::Text, Qt::white);
  darkPalette.setColor(QPalette::Disabled, QPalette::Text, Qt::gray);
  darkPalette.setColor(QPalette::Button, QColor(53,53,53));
  darkPalette.setColor(QPalette::ButtonText, Qt::white);
  darkPalette.setColor(QPalette::BrightText, Qt::red);
  darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
  darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
  darkPalette.setColor(QPalette::HighlightedText, Qt::black);
}

int		main(int argc, char *argv[])
{
  try {
    QApplication a(argc, argv);
    app = &a;
    GuiStrStyle s;
    guiStyle = &s;
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette	darkPalette;
    darkStyle(darkPalette);
    a.setPalette(darkPalette);
    a.setStyleSheet(guiStyle->contact()+guiStyle->lineEdit());
    Babel	w;
    a.exec();
  } catch (std::runtime_error&) {
        return (0);
  } catch (std::exception& e) {
     std::cerr << "Something append: " << e.what() << std::endl;
     return (1);
  }
  return 0;
}
