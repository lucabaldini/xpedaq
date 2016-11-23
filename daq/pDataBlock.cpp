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
    //std::cout << "**** " << header(evt) << std::endl;
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


/*!Destructor. Delete the underlying data buffer.
*/
pDataBlock::~pDataBlock()
{
  if (m_rawBuffer) delete[] m_rawBuffer;
}


/*!The amount of usable data in the buffer read from the USB port depends on
  the readout mode and is effectively:
  - 2 * NWORDS = 2 * 13200 * 8 = 211200 bytes in full frame mode (exactly one
  event).
  - < 10000 bytes in windowed mode with the small buffer (with the last event
  being truncated).
  - < 2 * 2**18 = 2* 262144 = 2 * SRAM_DIM = 524288 bytes (with the last event
  being truncated).
*/
pDataBlock::pDataBlock(const pDataBlock &cSourceDataBlock) :
  m_isWindowed (cSourceDataBlock.m_isWindowed)
{
  m_size = cSourceDataBlock.m_size;
  if (cSourceDataBlock.m_rawBuffer) {
    m_rawBuffer = new unsigned char[m_size];
    memcpy(m_rawBuffer, cSourceDataBlock.m_rawBuffer, m_size);
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
  if (!m_isWindowed) return;
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
  if (!m_isWindowed) return xpoldetector::kNumPixelsX - 1;
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
  if (!m_isWindowed) return xpoldetector::kNumPixelsY - 1;
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
microsecond_t pDataBlock::microseconds(unsigned int event) const
{
  return dataWord(event, Microseconds) +
    65534*dataWord(event, Microseconds + 2);
}


/*!

 */
double pDataBlock::timestamp(unsigned int event) const
{
  return dataWord(event, Seconds + 2) + 65536*dataWord(event, Seconds) +
    0.8e-6*microseconds(event);
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
adc_count_t pDataBlock::pixelCounts(unsigned int event,
				    unsigned int index) const
{
  /* TODO: we should check that index < numPixels() and throw an
     exception in case it is not.
  */
  if (m_isWindowed) return dataWord(event, AdcStart + 2*index);
  return dataWord(event, 2*index);
}


void pDataBlock::readPixel(unsigned int event, unsigned int index,
			   unsigned int &x, unsigned int &y,
			   adc_count_t &height) const
{
  static const unsigned int bufferHeight = xpoldetector::kNumPixelsY
                                   / xpoldetector::kNumReadOutBuffers;
  unsigned int xMin = xmin(event);
  unsigned int xMax = xmax(event);
  unsigned int yMin = ymin(event);    
  unsigned int nCol = xMax - xMin + 1;
  height = pixelCounts(event, index);
  if (m_isWindowed) {
    x = xMin + index % nCol;
    y = yMin + index / nCol;
    return;
  }
  unsigned int currBuffId =  index % xpoldetector::kNumReadOutBuffers;
  index = index / xpoldetector::kNumReadOutBuffers;
  x = index % nCol;
  y = index / nCol + bufferHeight * currBuffId;
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
