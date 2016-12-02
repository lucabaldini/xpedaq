/***********************************************************************
Copyright (C) 2007--2016 the X-ray Polarimetry Explorer (XPE) team.

For the license terms see the file LICENSE, distributed along with this
software.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
***********************************************************************/

#include <string>
#include <stdlib.h>
#include <iostream>
#include <QApplication>
#include <QtCore>
#include <QString>

#include "xpedaqutils.h"
#include "pOptionParser.h"
#include "pedviewerWindow.h"
#include "pedFileIO.h"
#include "pedestalsMap.h"

int main(int argn, char *argv[])
{
  xpedaqutils::startmsg();
  
  std::string intent = "View pedestal events from file";
  std::string usage = "xpepeds [options] filepath";
  pOptionParser parser(intent, usage);
  
  // Add the actual options.
  parser.addOption<std::string>("filepath", 'f',
                        "path to the input file");
  parser.addOption<std::string>("reference-file", 'r',
                                "Path to the reference pedestal file");
                        
  // Parse the command-line arguments.
  parser.parse(argn, argv);
  
  // Apply all command-line options.
  std::string filePath;
  if (!parser.optionSet("filepath")) {
    std::cout << "ERROR: No input file specified." << std::endl;
    std::cout << "Exiting." << std::endl;
    exit(1);
  } else {
    filePath = parser.value<std::string>("filepath");
  }
   
  //Read the pedestal map from file
  PedestalsMap pedMap = PedestalsMap();
  fillPedMapFromFile(pedMap, filePath);
  
  if (parser.optionSet("reference-file")) {
    std::string referenceMapFilePath  =  parser.value<std::string>
                                                          ("reference-file");
    PedestalsMap referenceMap = PedestalsMap();
    fillPedMapFromMapFile(referenceMap, referenceMapFilePath);
  }
  
  // Start the application.
  QApplication app(argn, argv);
  
  // Create the window.
  pedviewerWindow* window = new pedviewerWindow();
  QObject::connect(window, SIGNAL(windowClosed()),
	                 &app, SLOT(quit()));
  // Show the window content
  window -> show();
  window->showPedestals(pedMap);
  return app.exec();
}
