#include <QApplication>
#include <QString>

#include "xperegWindow.h"
#include "xpedaqutils.h"
#include "pOptionParser.h"


int main(int argn, char *argv[])
{
  xpedaqutils::startmsg();
 
  std::string intent = "Start the XPE Pedestal application";
  std::string usage = "xpereg [options]";
  pOptionParser parser(intent, usage);
  
  // Add the actual options.
  parser.addOption<int>("max-seconds", 's',
                        "Maximum data acquisition time (in s)");
  parser.addOption<bool>("batch", 'b',
                         "Run in batch mode");
  
  std::string cfgFolderPath = xpedaqos::rjoin("xpereg", "config");
  std::string configFilePath = xpedaqos::join(cfgFolderPath, "detector.cfg");
  std::string preferencesFilePath = xpedaqos::join(cfgFolderPath,
						   "preferences.cfg");
  std::string trgMaskFilePath = xpedaqos::join(cfgFolderPath, "trgmask.cfg");
  xperegRunController *runController = new xperegRunController();

  // Parse the command-line arguments.
  parser.parse(argn, argv);

  // Apply all command-line options.
  if (parser.optionSet("max-seconds")) {
    const int max_seconds = parser.value<int>("max-seconds");
    runController->setMaxSeconds(max_seconds);
  }
  bool batch = parser.value<bool>("batch");
  
  QApplication app(argn, argv);
  
  // Create the window.
  // Note that eventually we would like to move this inside the if statement
  // below but we can't due to issue #129.
  xperegWindow window(*runController);

  if (!batch) {
    window.show();    
  } else {
    QObject::connect(runController->dataCollector(), SIGNAL(finished()),
	 &app, SLOT(quit()));
    runController->setRunning();
  }
  return app.exec();
}

