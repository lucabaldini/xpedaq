/***********************************************************************
Copyright (C) 2007, 2008 by Luca Baldini (luca.baldini@pi.infn.it),
Johan Bregeon, Massimo Minuti and Gloria Spandre.

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

#include "pDataBlock.h"

/*!
  In this case the structure of the data block is known a-priori, since
  the event length is fixed. There are exactly two events per data block
  and, assuming that each event is <tt>NWORDS</tt> long, three indexes are
  needed, located at <tt>0</tt>, <tt>NWORDS</tt> and <tt>2*NWORDS</tt>.
*/

pDataBlock::pDataBlock(unsigned char *rawDataBlock)
{ 
  m_headerPositions.push_back(0);
  m_headerPositions.push_back(NWORDS);
  m_headerPositions.push_back(NWORDS*2);
  m_rawDataBlock = rawDataBlock;
}

/*!
  This case is more tricky since the event size is variable and not
  known a-priori. Also the buffer size (overall dimension of the raw data
  block) is adjustable and passed to the constructor as a parameter.
  The number of events therein is variable and most likely the last one
  is truncated; this must be taken into account when writing the data file.
  
  The entire data block is scanned and all the event headers found
  are put into the \ref m_headerPositions variable.
  
  \todo Move the hard-coded 0xff into a header file (most likely the
  xpoldetector namespace).
  
  \todo Move the printInfo() outside the constructor. It should be called
  explicitely and only if needed.
*/

pDataBlock::pDataBlock(unsigned char *rawDataBlock, int bufferSize)
{ 
  for (int i=0; i<bufferSize-1; i+=2)
    {
      if(rawDataBlock[i] == 0xff && rawDataBlock[i+1] == 0xff)
	{
	  m_headerPositions.push_back(i);
	} 	 
    }
  m_rawDataBlock = rawDataBlock;
}

/*!
  Effectively the function returns the last index in the
  \ref m_headerPositions vector, which points to the end of the last event.
  The rest of the data block, if any, is garbage and must be thrown away.
*/

int pDataBlock::getSize()
{
  if(m_headerPositions.empty())
    {
      return 0;
    }
  return m_headerPositions.back();
}

/*!
  The number of events is effectively the number of elements in
  \ref m_headerPositions, decremented by one unit.
*/

int pDataBlock::getNumEvents()
{
  if(m_headerPositions.empty())
    {
      return 0;
    }
  return m_headerPositions.size() - 1;
}

/*!
  The data word is properly byte-swapped.

  \param eventId The event number in the data block.
  \param offser The offset (in bits) from the event header.
*/

int pDataBlock::getDataWord(int eventId, int offset)
{
  if (eventId < 0)
    {
      eventId = 0;
    }
  else if (eventId >= getNumEvents())
    {
      eventId = getNumEvents() - 1;
    }
  return _BITESWAP_(m_rawDataBlock[m_headerPositions[eventId] + offset],
		    m_rawDataBlock[m_headerPositions[eventId] + offset + 8]); 
}

double pDataBlock::getTimestamp(int eventId)
{
  return 0.8e-6*(getDataWord(eventId, 96) + 65534*getDataWord(eventId, 112));
}

/*!
  The average event rate is defined by the total number of events in the data
  block divided by the time spanned between the timestamp of the first event
  and the timestamp of the last one in the data block.
 */

double pDataBlock::getAverageEventRate()
{
  double elapsedTime = getTimestamp(getNumEvents()-1) - getTimestamp(0);
  if (elapsedTime == 0.0)
    {
      return 0.0;
    }
  return getNumEvents()/elapsedTime;
}
