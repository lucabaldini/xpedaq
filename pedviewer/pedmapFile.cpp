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

#include "pedmapFile.h"

/*
*/
PedmapFile::PedmapFile(std::string filePath) :
  PedFile(filePath)
{
  m_fileType = pedmapType;
  *xpollog::kInfo << "Opening pedestals map file... " << endline;
  // Skip the header
  xpolio::kIOManager->skipLine(m_inputFile);
  readNumberOfEvents();
}


/*
*/
void PedmapFile::fillPedMap(PedestalsMap& map) const
{
  // Skip the map header
  xpolio::kIOManager->skipLine(m_inputFile);
  // Read average and rms for each pixel and fill the reference map
  for (unsigned int row=0; row < xpoldetector::kNumPixelsY; ++row){
    for (unsigned int col=0; col < xpoldetector::kNumPixelsX; ++col){
      double average = xpolio::kIOManager->readDouble(m_inputFile, false);
      double rms = xpolio::kIOManager->readDouble(m_inputFile, false);
      map.setPixel(col, row, m_nEvents, average, rms);
    }
    m_inputFile->ignore(1, '\n');
  }
}


/*
*/
void PedmapFile::readNumberOfEvents()
{
  // Skip the number of events header
  xpolio::kIOManager->skipLine(m_inputFile);
  // Read the number of events
  m_nEvents = xpolio::kIOManager->readInteger(m_inputFile);
  *xpollog::kInfo << "Input map has " << m_nEvents << " events"
                  << endline;
}

  
