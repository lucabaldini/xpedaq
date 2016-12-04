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
#include "pedFile.h"
#include "pedmapFile.h"
#include "pedDataFile.h"
#include "pedestalsMap.h"
#include "xpollog.h"


/* Application for viewing a file produced by the pedestals application
   (aka "xpepeds").
   Can be used to open:
   1) a ".pmap" file 
   2) a ".mdat" file
   If the input is a ".pmap" file, the application will load the map in the
   file and show its content.
   If the input is a ".mdat" file, the application will loop over the events
   in the files and fill a new map, that is than shown. The user is asked
   to sepcify the number of events (default=1) that will be included in the
   loop, as well as the starting event (default is to sart from the beginning
   of the file).
   A reference pedestal map (".pmap" file) can be passed as optional
   arguments. In that case, instead of the absolute values read from the file,
   the application will show a comparison with the reference map.  
*/
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
  PedFile* inputFile;
  std::string filePath;
  if (!parser.optionSet("filepath")) {
    std::cout << "ERROR: No input file specified." << std::endl;
    std::cout << "Exiting." << std::endl;
    exit(1);
  } else {
    filePath = parser.value<std::string>("filepath");
  }  
  PedFile::inputFileType fileType = identifyFileType(filePath);
  if (fileType == PedFile::pedmapType){
    inputFile = new PedmapFile(filePath);
  } else if (fileType == PedFile::dataType){
    inputFile = new PedDataFile(filePath);
  } else {
    *xpollog::kError << "Unknown file type: " << filePath << "Exiting..."
                    << endline;
    exit(1);
  }
  
  PedmapFile* referenceFile;
  if (parser.optionSet("reference-file")) {
    referenceFile = new PedmapFile(
                                 parser.value<std::string>("reference-file"));
  }
   
  //Read the pedestal map from file
  PedestalsMap pedMap = PedestalsMap();
  inputFile->fillPedMap(pedMap);
  
  // Start the application.
  QApplication app(argn, argv);
  
  // Create the window.
  pedviewerWindow* window = new pedviewerWindow();
  QObject::connect(window, SIGNAL(windowClosed()),
	                 &app, SLOT(quit()));
  // Show the window
  window -> show();
  
  if (!referenceFile){
    window->showPedestals(pedMap);
  } else {
    PedestalsMap referenceMap = PedestalsMap();
    referenceFile->fillPedMap(referenceMap);
    window->showPedestals(pedMap, referenceMap);
  }
  return app.exec();
}
