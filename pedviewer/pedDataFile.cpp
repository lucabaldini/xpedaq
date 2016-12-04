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

#include <sstream>
#include <iostream>

#include "pedDataFile.h"

/*
*/
PedDataFile::PedDataFile(std::string filePath) :
  PedFile(filePath)
{
  m_fileType = dataType;
  *xpollog::kInfo << "Opening data file... " << endline;
  readNumberOfEvents();
}


/*!
 */
void PedDataFile::fillPedMap(PedestalsMap& map) const
{
  int nEvents=1;
  int firstEvent=0;
  *xpollog::kInfo << "Reading full frame events... " << endline;    
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
  
  for (int evt = firstEvent; evt < (nEvents+firstEvent); ++evt){
    addEventToMap(map, evt);
  }
}


/*
*/
void PedDataFile::readNumberOfEvents()
{
  m_nEvents = fileSize()/(2*xpoldetector::kNumPixels);
  *xpollog::kInfo << "Input file has " << m_nEvents << " events"
                  << endline;
}


/* Add to the input map a specific event in the file. Leave the cursor
   at the end of the specified event
*/
void PedDataFile::addEventToMap(PedestalsMap& map, int evt) const
{
  // Move the cursor to the beginning of the event
  m_inputFile->seekg (evt*2*xpoldetector::kNumPixels, m_inputFile->beg);
  //Read the event
  addNextEventToMap(map);
}


/*! Add to the input map the event at the current stream position
 */
void PedDataFile::addNextEventToMap(PedestalsMap& map) const
{
  char* buffer = new (std::nothrow) char[SRAM_DIM*2];
  if (buffer == nullptr){
    *xpollog::kError << "Allocation failed" << endline;
    return;
  }
  m_inputFile->readsome(buffer, 2*xpoldetector::kNumPixels);
  pDataBlock block = pDataBlock(reinterpret_cast<unsigned char*>(buffer));
  addDataBlockToMap(map, block);
}


/*!
 */
void PedDataFile::addDataBlockToMap(PedestalsMap& map,
                                    const pDataBlock &block) const
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
