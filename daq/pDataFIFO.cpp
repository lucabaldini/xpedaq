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

#include "pDataFIFO.h"

/*! \param outputFilePath
  Path to the output binary file to which the data are written when the
  \ref pDataFIFO::flush() method is called.
  \param preferences
  pUserPreferences object; it is passed by the main GUI and contains all
  the relevant information about the multicast settings.
  
  \todo We should probably restructure this class in such a way the
  flush() method is automatically called at some point.
  I propose adding a m_maximumSize member, a isFull() method:
  \verbatim
  pDataFIFO::isFull()
  {
    return getNumDataBlocks() > m_maximumSize;
  }
  \endverbatim
  and an if statement into the fill() method sounding like
  \verbatim
  if isFull()
    {
    flush();
    }
  \endverbatim
  Not sure whether the isFull method should check the number of blocks, the
  number of events or the size.

  \todo Think about whether the flushing should be multi-threaded, at some
  point. We may gain speed, though I'm not sure.
*/

pDataFIFO::pDataFIFO(std::string outputFilePath,
		     pUserPreferences *preferences):
  m_numEvents(0),
  m_size(0),
  m_acquiredDataBlocks(0),
  m_acquiredEvents(0),
  m_outputFilePath(outputFilePath)
{
  m_multicastEnabled = preferences->multicastEnabled();
  if (m_multicastEnabled) {
    setHostAddress(preferences->multicastAddress());
    setPort(preferences->multicastPort());
  }
}

/*! The data block is pushed back into the FIFO and the event counter is
  properly incremented. If the multicast is enabled the datagrams are
  multicasted on the network.

  \todo Here the pUdpSocket::broadcastEventByEvent() is called.
  We can probably do something smarter, here like multicastring the the data
  in packets of event. Need to understand the impact on the DAQ.
*/

void pDataFIFO::fill(pDataBlock *dataBlock)
{
  m_buffer.push_back(dataBlock);
  m_numEvents += dataBlock->numEvents();
  m_size += dataBlock->size();
  m_acquiredDataBlocks += 1;
  m_acquiredEvents += dataBlock->numEvents();
  if (m_multicastEnabled) {  
    broadcastEventByEvent(dataBlock);
  }
}

void pDataFIFO::setStartSeconds(unsigned int startSeconds)
{
  for (m_iterator = m_buffer.begin(); m_iterator != m_buffer.end();
       m_iterator++) {
    (*m_iterator)->setStartSeconds(startSeconds);
  }
}

void pDataFIFO::flush(bool writeToDisk)
{
  if (writeToDisk){
    std::ofstream *outputFile = xpolio::kIOManager->
      openOutputFile(m_outputFilePath, true, true);
    for (m_iterator = m_buffer.begin(); m_iterator != m_buffer.end();
         m_iterator++) {
      outputFile->write((*m_iterator)->getCharDataBlock(),
		        (*m_iterator)->size());
    }
  xpolio::kIOManager->closeOutputFile(outputFile);
  }
  m_buffer.clear();
  m_numEvents = 0;
  m_size = 0;
}

