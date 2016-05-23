#include <QApplication>

#include "pOptionParser.h"
#include "xpedaqWindow.h"
#include "xpedaqutils.h"


int main(int argn, char *argv[])
{
  xpedaqutils::startmsg();

  // Setup the option parser.
  std::string intent = "Start the XPE DAQ";
  std::string usage = "xpedaq [options]";
  pOptionParser parser(intent, usage);
  // Add the actual options.
  parser.addOption<int>("max-seconds", 's',
                        "Maximum data acquisition time (in s)");
  parser.addOption<int>("max-events", 'n',
                        "Maximum number of events to be aquired");
  parser.addOption<int>("max-blocks", 'N',
                        "Maximum number of data buffers to be aquired");
  parser.addOption<bool>("batch", 'b',
			 "Run in batch mode");
  // Parse the command-line arguments.
  parser.parse(argn, argv);
  
  QApplication app(argn, argv);
  std::string cfgFolderPath = xpedaqos::rjoin("xpedaq", "config");
  std::string configFilePath = xpedaqos::join(cfgFolderPath, "detector.cfg");
  std::string preferencesFilePath = xpedaqos::join(cfgFolderPath,
						   "preferences.cfg");
  std::string trgMaskFilePath = xpedaqos::join(cfgFolderPath, "trgmask.cfg");
  pRunController *runController = new pRunController(configFilePath,
						     preferencesFilePath,
						     trgMaskFilePath);

  // Apply all command-line options.
  if (parser.optionSet("max-seconds")) {
    const int max_seconds = parser.value<int>("max-seconds");
    runController->setMaxSeconds(max_seconds);
  }
  if (parser.optionSet("max-events")) {
    const int max_events = parser.value<int>("max-events");
    runController->setMaxEvents(max_events);
  }
  if (parser.optionSet("max-blocks")) {
    const int max_blocks = parser.value<int>("max-blocks");
    runController->setMaxDataBlocks(max_blocks);
  }
  bool batch = parser.value<bool>("batch");

  // Create the window.
  xpedaqWindow window(*runController);
  if (!batch) {
    window.show();
  }
  return app.exec();
}

