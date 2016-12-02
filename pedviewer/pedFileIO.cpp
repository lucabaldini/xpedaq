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
#include <iostream>
#include <sstream>

#include "pedFileIO.h"
#include "xpoldetector.h"
#include "xpolio.h"
#include "xpedaqos.h"
#include "xpollog.h"


/*! Check if a file name ends with a given string
*/
bool nameEndsWith(std::string filePath, std::string ending)
{
  if (filePath.length() >= ending.length()) {
    return (0 == filePath.compare (filePath.length() - ending.length(),
            ending.length(), ending));
  } else {
    return false;
  }
}


/*!
 */
void fillPedMapFromMapFile(PedestalsMap& map, std::string filePath)
{
  *xpollog::kInfo << "Reading pedestals map from " << filePath
                  << "... " << endline;
  std::ifstream *inputFile = xpolio::kIOManager-> openInputFile(filePath);
  // Skip the header
  xpolio::kIOManager->skipLine(inputFile);
  // Skip the number of events header
  xpolio::kIOManager->skipLine(inputFile);
  // Read the number of events
  int nEntries = xpolio::kIOManager->readInteger(inputFile);
  *xpollog::kInfo << "Input map has " << nEntries << " events"
                  << endline;
  // Skip the map header
  xpolio::kIOManager->skipLine(inputFile);
  // Read average and rms for each pixel and fill the reference map
  for (unsigned int row=0; row < xpoldetector::kNumPixelsY; ++row){
    for (unsigned int col=0; col < xpoldetector::kNumPixelsX; ++col){
      double average = xpolio::kIOManager->readDouble(inputFile, false);
      double rms = xpolio::kIOManager->readDouble(inputFile, false);
      map.setPixel(col, row, nEntries, average, rms);
    }
    inputFile->ignore(1, '\n');
  }
  xpolio::kIOManager->closeInputFile(inputFile);
  *xpollog::kInfo << "Done." << endline;
}


/*!
 */
void fillPedMapFromBlock(PedestalsMap& map, const pDataBlock &block)
{
  for (unsigned int evt = 0; evt < block.numEvents(); ++evt) { 
    unsigned int x;
    unsigned int y;
    adc_count_t height = 0;
    for (unsigned int index = 0; index < block.numPixels(evt); ++index) {
      block.readPixel(evt, index, x, y, height);
      map.fill(x, y, height);
    }
  }
}


/*!
 */
void fillPedMapFromDataFile(PedestalsMap& map, std::string filePath,
                            int nEvents, int firstEvent)
{
  std::ifstream fStream;
  int fileErr; //file error status
  fStream.open(filePath.c_str(), std::ios::in | std::ios::binary );
  fileErr = fStream.bad();
  if (fileErr) {
    *xpollog::kError << "Could not open input file. Exiting..."
                    << endline;
    exit(1);
  }
  for (int evt = 0; evt<(nEvents+firstEvent); ++evt){
    char tmp;
    unsigned char* buffer = new (std::nothrow) unsigned char[SRAM_DIM*2];
    if (buffer == nullptr){
      *xpollog::kError << "Allocation failed" << endline;
      exit(1); 
    }
    for(unsigned int i=0; i < (2*map.nPedestals()); ++i){
      fStream.read((char*)&tmp, sizeof(char));
      buffer[i]  = tmp;
    }
    pDataBlock block = pDataBlock(buffer);
    if (evt >= firstEvent){
      fillPedMapFromBlock(map, block);
    }
  }
  fStream.close();
}


/*
*/
void fillPedMapFromFile(PedestalsMap& map, std::string filePath)
{
  if (!xpedaqos::fileExists(filePath)) {
    *xpollog::kError << "File not found: " << filePath
                     << endline;
    exit(1);
  }
  if (nameEndsWith(filePath, ".pmap")){
    fillPedMapFromMapFile(map, filePath);
  } else if (nameEndsWith(filePath, ".mdat")){
    int nEvents=1;
    int firstEvent=0;
    *xpollog::kInfo << "Reading full frame events from " << filePath
                  << "... " << endline;    
    std::cout << "Select the number of events to be read "
              << "[default = 1]: ";
    std::string strInput = "";
    while (true){
      getline(std::cin, strInput);
      // This code converts from string to number safely.
      std::stringstream inputStream(strInput);
      if (inputStream.str() == "" || (inputStream >> nEvents)){
        break;
      }
      std::cout << "Invalid input, please try again" << std::endl;
    }
    std::cout << nEvents << std::endl;
    std::cout << "Select the first event to be read [default = 0 "
                 << "(first event)]: ";
    strInput = "";
    while (true){
      getline(std::cin, strInput);
      // This code converts from string to number safely.
      std::stringstream inputStream(strInput);
      if (inputStream.str() == "" || (inputStream >> firstEvent)){
        break;
      }
      std::cout << "Invalid input, please try again" << std::endl;
    }
    fillPedMapFromDataFile(map, filePath, nEvents, firstEvent);
  } else {
    *xpollog::kError << "Input file does not look like a valid file: "
                     << filePath << endline;
    exit(1);
  }
}
