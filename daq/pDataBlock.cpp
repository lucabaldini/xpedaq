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
  Full frame mode (one event with fixed length).
*/

pDataBlock::pDataBlock(unsigned char *buffer) :
  m_rawBuffer(buffer),
  m_size(2*NWORDS),
  m_errorSummary(0),
  m_isWindowed(false)
{ 
  m_offsetVec.push_back(0);
}

/*!
  Window-mode (events with variable length).
*/

pDataBlock::pDataBlock(unsigned char *buffer, unsigned int bufferSize) :
  m_rawBuffer(buffer),
  m_errorSummary(0),
  m_isWindowed(true)
{
  unsigned int pos = 0;
  unsigned int evt = 0;
  while (pos < bufferSize) {
    m_offsetVec.push_back(pos);
    if (header(evt) != 0xffff) {
      m_errorSummary += IdMismatch;
    }
    if (xmin(evt) >= xpoldetector::kNumPixelsX) {
      m_errorSummary |= UnphysicalXMin;
    }
    if (xmax(evt) >= xpoldetector::kNumPixelsX) {
      m_errorSummary |= UnphysicalXMax;
    }
    if (ymin(evt) >= xpoldetector::kNumPixelsY) {
      m_errorSummary |= UnphysicalYMin;
    }
    if (ymax(evt) >= xpoldetector::kNumPixelsY) {
      m_errorSummary |= UnphysicalYMax;
    }
    pos += AdcStart + 2*numPixels(evt);
    evt += 1;
  }
  m_size = pos;
}


pDataBlock::pDataBlock(const pDataBlock &cSourceDataBlock) :
		          m_isWindowed (cSourceDataBlock.m_isWindowed)
{
  m_size = cSourceDataBlock.m_size;
  if (cSourceDataBlock.m_rawBuffer)
  {
    /* Awful. Isn't there a better way to copy the buffer? */    
    char* temp_buffer = new char [2*NWORDS];
    memcpy(temp_buffer, cSourceDataBlock.getCharDataBlock(), m_size);
    m_rawBuffer = reinterpret_cast<unsigned char*> (temp_buffer);
  }
  m_errorSummary = cSourceDataBlock.m_errorSummary;
  m_offsetVec = cSourceDataBlock.m_offsetVec;
}

/*!
  Overwrite the 4 unused bytes in the event header to add the number of
  seconds since January 1, 1970 at the start time (poor man's attemp at an
  absolute timestamp).
*/
void pDataBlock::setStartSeconds(unsigned int startSeconds)
{
  if (errorSummary() == 0) {
    for (unsigned int evt = 0; evt < numEvents(); evt ++) {
      unsigned int offset = m_offsetVec[evt] + Seconds;
      m_rawBuffer[offset + 0] = (startSeconds & 0x00ff0000) >> 16;
      m_rawBuffer[offset + 1] = (startSeconds & 0xff000000) >> 24;
      m_rawBuffer[offset + 2] = (startSeconds & 0x000000ff) >> 0;
      m_rawBuffer[offset + 3] = (startSeconds & 0x0000ff00) >> 8;
    }
  }
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
unsigned int pDataBlock::header(unsigned int event) const
{
  if (!m_isWindowed) return 0;
  return dataWord(event, Header);
}

/*!

 */
unsigned int pDataBlock::xmin(unsigned int event) const
{
  if (!m_isWindowed) return 0;
  return dataWord(event, WindowXMin);
}

/*!

 */
unsigned int pDataBlock::xmax(unsigned int event) const
{
  if (!m_isWindowed) return xpoldetector::kNumPixelsX;
  return dataWord(event, WindowXMax);
}

/*!

 */
unsigned int pDataBlock::ymin(unsigned int event) const
{
  if (!m_isWindowed) return 0;
  return dataWord(event, WindowYMin);
}

/*!

 */
unsigned int pDataBlock::ymax(unsigned int event) const
{
  if (!m_isWindowed) return xpoldetector::kNumPixelsY;
  return dataWord(event, WindowYMax);
}

/*!

 */
unsigned int pDataBlock::bufferId(unsigned int event) const
{
  if (!m_isWindowed) return 0;
  return dataWord(event, BufferId);
}

/*!

 */
unsigned int pDataBlock::numPixels(unsigned int event) const
{
  if (!m_isWindowed) return xpoldetector::kNumPixelsX * xpoldetector::kNumPixelsY;
  return (xmax(event) - xmin(event) + 1)*(ymax(event) - ymin(event) + 1);
}

/*!

 */
double pDataBlock::timestamp(unsigned int event) const
{
  return dataWord(event, Seconds + 2) + 65536*dataWord(event, Seconds) +
    0.8e-6*(dataWord(event, Microseconds) + 
	    65534*dataWord(event, Microseconds + 2));
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

/*
*/
unsigned int pDataBlock::pixelCounts(unsigned int event,
                                     unsigned int index) const
{
  /* TODO: we probably need to check that index < numPixels() and throw an
     exception in case it is not.
  */
  return dataWord(event, AdcStart*m_isWindowed + 2*index);
}


/*
*/
pEvent pDataBlock::event(unsigned int evtIndex)
{
  
  unsigned int startIndex = offset(evtIndex);
  unsigned int buffSize = 2*numPixels(evtIndex);
  std::vector<unsigned int> adcCounts;
  for (unsigned int index = startIndex + AdcStart;
       index < startIndex + AdcStart + buffSize;
       index+=2)
  {
    adcCounts.push_back(dataWord(index));
  }
  return pEvent(xmin(evtIndex), xmax(evtIndex),
                ymin(evtIndex), ymax(evtIndex),
                bufferId(evtIndex), adcCounts);
}

/*
*/
std::vector<pEvent> pDataBlock::events()
{
  unsigned int nEvt = numEvents();
  std::vector<pEvent> evtVect;
  for (unsigned int i =0; i < nEvt; i++)
    {evtVect.push_back(event(i));}
  return evtVect;
}


/*!
  Terminal formatting.
 */
std::ostream& pDataBlock::fillStream(std::ostream& os) const
{
  os << "pDataBlock object with " << numEvents() << " event(s) in " << size()
     << " bytes (error summary 0x" << std::hex << errorSummary() << std::dec
     << ")." << std::endl;
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
