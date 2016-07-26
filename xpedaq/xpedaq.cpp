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
  parser.addOption<int>("clock-shift", 'c',
                        "Clock shift code (0-800 in step of 25)");
  parser.addOption<int>("clock-frequency", 'f',
                        "Clock frequency code (0-32-64-96)");
  parser.addOption<int>("nped-subtracted", 'p',
           "Number of sampling for pedestal subtraction (0-1-2-4-8)");                        
  parser.addOption<bool>("charge-injection", 'I',
                         "Charge injection mode");
  parser.addOption<int>("x-pixel-address", 'x',
                        "Charge injection pixel coordinate x");
  parser.addOption<int>("y-pixel-address", 'y',
                        "Charge injection pixel coordinate y");
  parser.addOption<int>("calibration-dac", 'C',
                  "Calibration DAC signal for charge injection mode");
  parser.addOption<std::string>("comment", 'm', "A user comment");                  
  

  std::string cfgFolderPath = xpedaqos::rjoin("xpedaq", "config");
  std::string configFilePath = xpedaqos::join(cfgFolderPath, "detector.cfg");
  std::string preferencesFilePath = xpedaqos::join(cfgFolderPath,
						   "preferences.cfg");
  std::string trgMaskFilePath = xpedaqos::join(cfgFolderPath, "trgmask.cfg");
  pRunController *runController = new pRunController(configFilePath,
						     preferencesFilePath,
						     trgMaskFilePath);
  pDetectorConfiguration* configuration =
                               runController->detectorConfiguration();

  // Parse the command-line arguments.
  parser.parse(argn, argv);

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
  if (parser.optionSet("nped-subtracted")){
    const int nped_subtracted = parser.value<int>("nped-subtracted");
    configuration->setNumPedSamples(nped_subtracted);
  }
  if (parser.optionSet("charge-injection")){
    const bool charge_injection = parser.value<bool>("charge-injection");
    if (charge_injection){
      configuration->setReadoutMode(xpoldetector::kChargeInjectionReadoutCode);
      if (parser.optionSet("x-pixel-address")) {
        const int x_pixel_address = parser.value<int>("x-pixel-address");
        configuration->setPixelAddressX(x_pixel_address);
      } else {
        *xpollog::kError << "Missing pixel address x in charge "
                         << "injection mode. Use option -x [int]."
                         << endline;
        return 1;
      }
      if (parser.optionSet("y-pixel-address")) {
        const int y_pixel_address = parser.value<int>("y-pixel-address");
        configuration->setPixelAddressY(y_pixel_address);
      } else {
        *xpollog::kError << "Missing pixel address y in charge "
                         << "injection mode. Use option -y [int]."
                         << endline;
        return 1;
      }
      if (parser.optionSet("calibration-dac")) {
        const int calibration_dac = parser.value<int>("calibration-dac");
        configuration->setCalibrationDac(calibration_dac);
      } else {
        *xpollog::kError << "No calibration signal intenisty specified "
                         << "in charge-injection mode. "
                         << "Use option -C [int]."
                         << endline;
        return 1;
      }
    }
  }
  if (parser.optionSet("comment")){
    const std::string userComment = parser.value<std::string>("comment");
    runController->setUserComment(userComment);
  }
  bool batch = parser.value<bool>("batch");

  QApplication app(argn, argv);

  // Create the window.
  xpedaqWindow window(*runController);
  if (!batch) {
    window.show();
  }
  else {
    window.startRun();
  }
  return app.exec();
}

