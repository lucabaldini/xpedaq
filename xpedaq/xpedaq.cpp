#include <QApplication>
#include <QString>
#include <QObject>

#include "xpedaqWindow.h"
#include "xpedaqutils.h"

int main(int argn, char *argv[])
{
  xpedaqutils::startmsg();
  
  //bool autostart = false;
  //bool batchmode = false;
  
  QApplication app(argn, argv);
  xpedaqWindow window;

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

