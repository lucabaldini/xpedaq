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
  Full frame mode (exactly two events with predetermined length in the buffer).
*/

pDataBlock::pDataBlock(unsigned char *buffer) :
  m_rawBuffer(buffer),
  m_size(2*NWORDS),
  m_errorSummary(0)
{ 
  m_offsetVec.push_back(0);
  m_offsetVec.push_back(NWORDS);
  for (unsigned int evt = 0; evt < 2; evt ++) {
    if (header(evt) != 0xffff) {
      m_errorSummary += 1;
    }
  }
}

/*!
  Window-mode (events with variable length).
*/

pDataBlock::pDataBlock(unsigned char *buffer, unsigned int bufferSize) :
  m_rawBuffer(buffer),
  m_errorSummary(0)
{
  unsigned int pos = 0;
  unsigned int evt = 0;
  while (pos < bufferSize) {
    m_offsetVec.push_back(pos);
    if (header(evt) != 0xffff) {
      m_errorSummary += 1;
    }
    pos += 20 + 2*numPixels(evt);
    evt += 1;
  }
  m_size = pos;
}

/*!
  
*/

unsigned int pDataBlock::dataWord(unsigned int offset) const
{
  return _BYTESWAP_(m_rawBuffer[offset], m_rawBuffer[offset + 1]); 
}

/*!
  
*/
unsigned int pDataBlock::dataWord(unsigned int event, unsigned int offset) const
{
  return dataWord(m_offsetVec[event] + offset);
}

/*!

 */
unsigned int pDataBlock::numPixels(unsigned int event) const
{
  return (xmax(event) - xmin(event) + 1)*(ymax(event) - ymin(event) + 1);
}

/*!

 */
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
  double elapsedTime = timestamp(numEvents() - 1) - timestamp(0);
  if (elapsedTime == 0.0) {
    return 0.0;
  }
  return numEvents()/elapsedTime;
}

/*!
  Terminal formatting.
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
