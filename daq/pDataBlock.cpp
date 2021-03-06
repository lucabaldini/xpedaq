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
  
  This function is under restructuring, as through the development of
  the new firmware we realized that the old logic was writing to disk
  the last event of the buffer, whosa data were located past the 
  bufferSize limit.
  
  In the new logic we:
  * update the m_size class member in the event loop, rather than at 
    the end of the loop.
  * have a break statement preventing the event counter to be updated
    past the hard bufferSize limit (in which case we also pop the last
	location out of the m_offsetVec class member.)
	
  Note that the variable pos in the event loop is pointing to the first
  location of memory for the next event, as opposed to the last location 
  of the previous event. Therefore the out-of-bound check reads 
  pos > bufferSize rather than pos >= bufferSize.
*/

pDataBlock::pDataBlock(unsigned char *buffer, unsigned int bufferSize) :
  m_rawBuffer(buffer),
  m_errorSummary(0),
  m_isWindowed(true)
{
  unsigned int pos = 0;
  unsigned int evt = 0;
  //std::cout << bufferSize << std::endl;
  while (pos < bufferSize) {
	m_size = pos;
	m_offsetVec.push_back(pos);
    //std::cout << "**** " << header(evt) << " " << pos << std::endl;
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
	if (pos > bufferSize) {
		m_offsetVec.pop_back();
	    break;
	}
    evt += 1;
  }
  //std::cout << m_size << " " << evt << " " << m_offsetVec.size() << std::endl;
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

void pDataBlock::mydebug(unsigned int event)  const
{
	std::cout <<  dataWord(event, BufferId) 
	<< " - " << dataWord(event, 12) << " - " << dataWord(event, 14) 
	<< " - " << dataWord(event, 16) << " - " << dataWord(event, 18)
	<< " - " << dataWord(event, 20) << " - " << dataWord(event, 22) << std::endl;
	
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


int pDataBlock::verifyRoi(unsigned int evt, unsigned short x,
			  unsigned short y) const
{
  int errorCode = 0;
  // Calculate the expected ROI.
  short int expXMin, expXMax, expYMin, expYMax;
  unsigned short padx = 8;
  unsigned short pady = 10;
  if (x % 2 == 0) {
    expXMin = x - padx;
    expXMax = x + padx + 1;
  } else {
    expXMin = x - padx - 1;
    expXMax = x + padx;
  }
  if (expXMin < 0) {
    expXMin = 0;
  }
  if (expXMax > xpoldetector::kNumPixelsX) {
    expXMax = xpoldetector::kNumPixelsX;
  }
  if (y % 2 == 0) {
    expYMin = y - pady;
    expYMax = y + pady + 1;
  } else {
    expYMin = y - pady - 1;
    expYMax = y + pady;
  }
  if (expYMin < 0) {
    expYMin = 0;
  }
  if (expYMax > xpoldetector::kNumPixelsY) {
    expYMax = xpoldetector::kNumPixelsY;
  }
  // Compare the actual ROI with the expected one.
  if (xmin(evt) != static_cast<unsigned short>(expXMin)) {
    errorCode += WrongXMin;
  }
  if (xmax(evt) != static_cast<unsigned short>(expXMax)) {
    errorCode += WrongXMax;
  }
  if (ymin(evt) != static_cast<unsigned short>(expYMin)) {
    errorCode += WrongYMin;
  }
  if (ymax(evt) != static_cast<unsigned short>(expYMax)) {
    errorCode += WrongYMax;
  }
  if (errorCode) {
    *xpollog::kInfo << "Expected ROI w(" << expXMin << ", " << expYMin
		    << ")--(" << expXMax << ", " << expYMax << "), got w("
		    << xmin(evt) << ", " << ymin(evt) << ")--(" << xmax(evt)
		    << ", " << ymax(evt) << ")" << endline;
  }
  return errorCode;
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
