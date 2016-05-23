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


  if (parser.optionSet("max-seconds")) {
    const int max_seconds = parser.value<int>("max-seconds");
    std::cout << "max_seconds: " << max_seconds << std::endl;
  }
  if (parser.optionSet("max-events")) {
    const int max_events = parser.value<int>("max-events");
    std::cout << "max_events: " << max_events << std::endl;
  }
  if (parser.optionSet("max-blocks")) {
    const int max_blocks = parser.value<int>("max-blocks");
    std::cout << "max_blocks: " << max_blocks << std::endl;
  }
  //std::cout << parser.value<bool>("batch") << std::endl;

  
  //bool autostart = false;
  //bool batchmode = false;
  
  QApplication app(argn, argv);
  std::string cfgFolderPath = xpedaqos::rjoin("xpedaq", "config");
  std::string configFilePath = xpedaqos::join(cfgFolderPath, "detector.cfg");
  std::string preferencesFilePath = xpedaqos::join(cfgFolderPath,
						   "preferences.cfg");
  std::string trgMaskFilePath = xpedaqos::join(cfgFolderPath, "trgmask.cfg");
  pRunController *runController = new pRunController(configFilePath,
						     preferencesFilePath,
						     trgMaskFilePath);
  xpedaqWindow window(*runController);

  for (int i = 1; i < argn; i++)
    {
      std::string option = argv[i];
      //if (option == "-s")
      //	{
      //	  autostart = true;
      //	}
      //else if (option == "-b")
      //{
      //  batchmode = true;
      //  autostart = true;
      //}
      if (option == "-t")
	{
	  i++;
	  window.runController()->setMaxSeconds(atoi(argv[i]));
	}
      else if (option == "-n")
	{
	  i++;
	  window.runController()->setMaxEvents(atoi(argv[i]));
	}
      else if (option == "-d")
	{
	  i++;
	  window.runController()->setMaxDataBlocks(atoi(argv[i]));
	}
      //else if (option == "-o")
      ///	{
      //  i++;
      //  window.getRunController()->setOutputFilePath(argv[i]);
      //	}
    }
  //if (batchmode)
  // {
  //  window.getRunController()->setBatch();
  // }
  //else
  // {
  window.show();
  //  }
  //if (autostart)
  //  {
  //    window.start();
  // }
  return app.exec();
}

