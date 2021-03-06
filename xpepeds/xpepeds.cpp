#include <QApplication>
#include <QString>

#include "xpepedsWindow.h"
#include "xpedaqutils.h"
#include "pOptionParser.h"


int main(int argn, char *argv[])
{
  xpedaqutils::startmsg();
 
  std::string intent = "Start the XPE Pedestal application";
  std::string usage = "xpepeds [options]";
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
  parser.addOption<int>("clock-shift", 'c',
                        "Clock shift code (0-800 in step of 25)");
  parser.addOption<int>("clock-frequency", 'f',
                        "Clock frequency code (0-32-64-96)");  
  parser.addOption<std::string>("comment", 'm', "A user comment");
  parser.addOption<std::string>("reference-file", 'r',
                                "Path to the reference pedestal file");
  parser.addOption<int>("sigma-threshold", 't',
                        "Threshold for outlier pixels (in sigma)");
  parser.addOption<int>("bad-pixels", 'p',
    "Number of outlier pixels required to tag an event as corrupted");
  // Parse the command-line arguments.
  parser.parse(argn, argv);

  std::string cfgFolderPath = xpedaqos::rjoin("xpepeds", "config");
  std::string configFilePath = xpedaqos::join(cfgFolderPath, "detector.cfg");
  std::string preferencesFilePath = xpedaqos::join(cfgFolderPath,
						   "preferences.cfg");
  std::string trgMaskFilePath = xpedaqos::join(cfgFolderPath, "trgmask.cfg");
  
  // Initialize run controller and detector configuration
  pedRunController *runController;
  runController = new pedRunController(configFilePath, preferencesFilePath,
                                       trgMaskFilePath);
  pDetectorConfiguration* configuration =
    runController->detectorConfiguration();  

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
  if (parser.optionSet("clock-shift")){
    const int clock_shift = parser.value<int>("clock-shift");
    configuration->setClockShift(clock_shift);
  }
  if (parser.optionSet("clock-frequency")){
    const int clock_frequency = parser.value<int>("clock-frequency");
    configuration->setClockFrequency(clock_frequency);
  }
  if (parser.optionSet("comment")){
    const std::string userComment = parser.value<std::string>("comment");
    runController->setUserComment(userComment);
  }
  if (parser.optionSet("reference-file")) {
    std::string referenceMapFilePath  =  parser.value<std::string>
                                                          ("reference-file");
    runController->loadRefMapFromFile(referenceMapFilePath);
    if (parser.optionSet("sigma-threshold")) {
      int nSigma = parser.value<int>("sigma-threshold");
      runController->setNSigmaAlarmThreshold(nSigma);
    }
    if (parser.optionSet("bad-pixels")) {
      int nBadPixels = parser.value<int>("bad-pixels");
      runController->setNBadPixelsThreshold(nBadPixels);
    }
  }
  bool batch = parser.value<bool>("batch");
  
  QApplication app(argn, argv);
  
  // Create the window.
  // Note that eventually we would like to move this inside the if statement
  // below but we can't due to issue #129.
  xpepedsWindow window(*runController);

  if (!batch) {
    window.show();    
  } else {
    QObject::connect(runController->dataCollector(), SIGNAL(finished()),
	 &app, SLOT(quit()));
    runController->setRunning();
  }
  return app.exec();
}

