#include <QApplication>
#include "xpemonWindow.h"
#include "xpedaqutils.h"

int main( int argc, char **argv )
{
  //QApplication::setGraphicsSystem("raster");
  QApplication app(argc, argv);
  std::string cfgFolderPath = xpedaqos::rjoin("xpemon", "config");
  std::string preferencesFilePath = xpedaqos::join(cfgFolderPath,
						   "preferences.cfg");
  if (!xpedaqos::fileExists(preferencesFilePath)) {
    xpedaqos::copyFile(preferencesFilePath + ".sample",
		               preferencesFilePath);
  }						   
  xpemonWindow w(preferencesFilePath);
  w.show();
  return app.exec();
}
