#include <QApplication>
#include "xpemonWindow.h"

int main( int argc, char **argv )
{
  //QApplication::setGraphicsSystem("raster");
  QApplication a(argc, argv);
  xpemonWindow w;
  w.show();
  return a.exec();
}
