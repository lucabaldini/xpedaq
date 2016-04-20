#include <QApplication>
#include <QString>
#include <QObject>

#include "pMainWindow.h"

int main(int argn, char *argv[])
{
  bool autostart = false;
  bool batchmode = false;
  
  QApplication app(argn, argv);
  pMainWindow window;

  for (int i = 1; i < argn; i++)
    {
      std::string option = argv[i];
      if (option == "-s")
	{
	  autostart = true;
	}
      else if (option == "-b")
	{
	  batchmode = true;
	  autostart = true;
	}
      else if (option == "-t")
	{
	  i++;
	  window.getRunController()->setMaxElapsedSeconds(atoi(argv[i]));
	}
      else if (option == "-n")
	{
	  i++;
	  window.getRunController()->setMaxAcquiredEvents(atoi(argv[i]));
	}
      else if (option == "-d")
	{
	  i++;
	  window.getRunController()->setMaxAcquiredDataBlocks(atoi(argv[i]));
	}
      else if (option == "-o")
	{
	  i++;
	  window.getRunController()->setOutputFilePath(argv[i]);
	}
      else if (option == "-h")
	{
	  i++;
	  window.getRunController()->setHeaderFilePath(argv[i]);
	}
    }
  if (batchmode)
    {
      window.getRunController()->setBatch();
    }
  else
    {
      window.show();
    }
  if (autostart)
    {
      window.start();
    }
  return app.exec();
}

