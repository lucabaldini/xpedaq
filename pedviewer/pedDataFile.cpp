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
int PedDataFile::fillPedMap(PedestalsMap& map) const
{
  for (int evt = 0; evt < m_nEvents; ++evt){
    addEventToMap(map, evt);
  }
  return m_nEvents;
}


/*!
 */
int PedDataFile::fillPedMap(PedestalsMap& map, int firstEvent,
                            int numEvents) const
{
  for (int evt = firstEvent; evt < (numEvents+firstEvent); ++evt){
    addEventToMap(map, evt);
  }
  return numEvents + firstEvent - 1;
}
                           

/*! \brief Note: no check is performed to make sure the given event numbers
           are within range
 */
void PedDataFile::fillPedMap(PedestalsMap& map,
                             const std::vector<int>& events) const
{
  for (auto evt: events){
    addEventToMap(map, evt);
  }
}


/*! \brief Note: event numbering goes from 1 to m_nEvents
 */
int PedDataFile::curEvent() const
{
  return 1 + (m_inputFile->tellg())/(nBytesPerEvent());
}


/* Determine the number of events in the file
*/
void PedDataFile::readNumberOfEvents()
{
  m_nEvents = fileSize()/(nBytesPerEvent());
  *xpollog::kInfo << "Input file has " << m_nEvents << " events"
                  << endline;
}


/* Move the cursor to the beginning of a specific event
*/
void PedDataFile::goToEvent(int evtNumber) const
{
  m_inputFile->seekg ((evtNumber-1)*nBytesPerEvent(), m_inputFile->beg);
}


/* Add to the input map a specific event in the file. Leave the cursor
   at the end of the specified event
*/
void PedDataFile::addEventToMap(PedestalsMap& map, int evtNumber) const
{
  // Move the cursor to the beginning of the event
  goToEvent(evtNumber);
  //Read the event
  addNextEventToMap(map);
}


/*! Add to the given map the event at the current stream position
 */
void PedDataFile::addNextEventToMap(PedestalsMap& map) const
{
  char* buffer = new (std::nothrow) char[SRAM_DIM*2];
  if (buffer == nullptr){
    *xpollog::kError << "Allocation failed" << endline;
    return;
  }
  m_inputFile->readsome(buffer, nBytesPerEvent());
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
