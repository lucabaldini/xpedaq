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

#include "pDataBlock.h"

/*!
  In this case the structure of the data block is known a-priori, since
  the event length is fixed. There are exactly two events per data block
  and, assuming that each event is <tt>NWORDS</tt> long, three indexes are
  needed, located at <tt>0</tt>, <tt>NWORDS</tt> and <tt>2*NWORDS</tt>.
*/

pDataBlock::pDataBlock(unsigned char *buffer) :
  m_bufferSize(2*NWORDS),
  m_errorSummary(0)
{ 
  m_eventBounds.push_back(0);
  m_eventBounds.push_back(NWORDS);
  m_eventBounds.push_back(NWORDS*2);
  m_rawBuffer = buffer;
}

/*!
  This case is more tricky since the event size is variable and not
  known a-priori. Also the buffer size (overall dimension of the raw data
  block) is adjustable and passed to the constructor as a parameter.
  The number of events therein is variable and most likely the last one
  is truncated; this must be taken into account when writing the data file.
  
  The entire data block is scanned and all the event headers found
  are put into the \ref m_eventBounds variable.
  
  \todo Move the hard-coded 0xff into a header file (most likely the
  xpoldetector namespace).
  
  \todo Move the printInfo() outside the constructor. It should be called
  explicitely and only if needed.
*/

pDataBlock::pDataBlock(unsigned char *buffer, int bufferSize) :
  m_bufferSize(bufferSize),
  m_errorSummary(0)
{ 
  for (int i = 0; i < bufferSize - 1; i += 2) {
    if (buffer[i] == 0xff && buffer[i+1] == 0xff) {
      m_eventBounds.push_back(i);
    }
  }
  m_rawBuffer = buffer;
}

/*!
  The data word is properly byte-swapped.

  \param event The event number in the data block.
  \param offset The offset (in bits) from the event header.
*/

unsigned int pDataBlock::dataWord(unsigned int event, unsigned int offset) const
{

  if (event >= numEvents()) {
    event = numEvents() - 1;
  }
  return _BYTESWAP_(m_rawBuffer[m_eventBounds[event] + offset],
		    m_rawBuffer[m_eventBounds[event] + offset + 1]); 
}

/*!

 */
unsigned int pDataBlock::numPixels(unsigned int event) const
{
  return (xmax(event) - xmin(event) + 1)*(ymax(event) - ymin(event) + 1);
}

double pDataBlock::timestamp(unsigned int event) const
{
  return 0.8e-6*(dataWord(event, 12) + 65534*dataWord(event, 14));
}

/*!
  The average event rate is defined by the total number of events in the data
  block divided by the time spanned between the timestamp of the first event
  and the timestamp of the last one in the data block.
 */

double pDataBlock::averageEventRate() const
{
  double elapsedTime = timestamp(numEvents()-1) - timestamp(0);
  if (elapsedTime == 0.0) {
    return 0.0;
  }
  return numEvents()/elapsedTime;
}

/*!

 */
std::ostream& pDataBlock::fillStream(std::ostream& os) const
{
  os << "pDataBlock object with " << numEvents() << " event(s) in " << size()
     << " bytes (error summary " << errorSummary() << ")." << std::endl;
  for (unsigned int evt = 0; evt < numEvents(); evt ++) {
    os << "#" << evt << " @ buf " << std::setfill('0') << std::setw(6)
       << bufferId(evt) << "+" << std::setw(6) << offset(evt) << ", w("
       << std::setfill(' ') << std::setw(3) << xmin(evt) << ", "
       << std::setw(3) << ymin(evt) << ")--(" << std::setw(3) << xmax(evt)
       << ", " << std::setw(3) << ymax(evt) << "), " << numPixels(evt)
       << " pixels, " << timestamp(evt) << " s" << std::endl;
  }
  return os;
}
