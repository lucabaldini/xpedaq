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

#ifndef PDATABLOCK_H
#define PDATABLOCK_H

#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "pUsbController.h"
#include "xpoldetector.h"


#define _BYTESWAP_(x1, x2) (((x2 & 0xff) << 8) | (x1 & 0xff))

/*! \brief Class describing a data block.

  A data block is essentially a block of data read from the FPGA
  through the USB port. At the low level the data block is a simple
  <tt>unsigned char *</tt> which may contain several events (how many
  exactly depends on the acquisition mode).
  A little bit of processing is performed in the constructors, though,
  as explained in the documentation. In particular the indexes of the
  event headers are stored since they are used for multicasting packets
  to the monitor and while writing to file.
*/

class pDataBlock
{
  
 public:

  pDataBlock(unsigned char *rawDataBlock);
  pDataBlock(unsigned char *rawDataBlock, unsigned int bufferSize);
  /* In order to allow the emission of an object as a signal Qt requires it to
     have public no-arg constructor and destructor and a copy constructor */
  pDataBlock():m_isWindowed(false){;}
  pDataBlock(const pDataBlock &cSourceDataBlock);
  ~pDataBlock();

  // These are used for the UDP socket. I am not sure they belong here.
  inline char *getCharDataBlock() const {return (char*)m_rawBuffer;}
  inline char *getCharDataBlock(int index)
    {return (char*)(m_rawBuffer + index);}

  inline unsigned char rawBuffer() const {return *m_rawBuffer;}
  inline std::vector<unsigned int> offsetVec() const {return m_offsetVec;}
  inline unsigned int errorSummary() const {return m_errorSummary;}
  inline unsigned int size() const {return m_size;}
  unsigned int numEvents() const {return m_offsetVec.size();}
  unsigned int offset(unsigned int event) const {return m_offsetVec[event];}
  unsigned int header(unsigned int event) const;
  unsigned int xmin(unsigned int event) const;
  unsigned int xmax(unsigned int event) const;
  unsigned int ymin(unsigned int event) const;
  unsigned int ymax(unsigned int event) const;
  unsigned int bufferId(unsigned int event) const;
  unsigned int numPixels(unsigned int event) const;
  microsecond_t microseconds(unsigned int event) const;
  double timestamp(unsigned int event) const;

  // Random access to the content of single pixels in an event
  adc_count_t pixelCounts(unsigned int event, unsigned int index) const;
  void readPixel(unsigned int event, unsigned int index,
                 unsigned int &x, unsigned int &y, adc_count_t &height) const;
  
  double averageEventRate() const;

  void setStartSeconds(unsigned int startSeconds);

  int verifyRoi(unsigned int evt, unsigned short x, unsigned short y) const;
 
  // Terminal formatting.
  std::ostream& fillStream(std::ostream& os) const;
  friend std::ostream& operator<<(std::ostream& os, const pDataBlock& block)
    {return block.fillStream(os);}

	/*
   */
  void mydebug(unsigned int event)  const;
  
 private:

  enum WordOffset {
    Header = 0,
    WindowXMin = 2,
    WindowXMax = 4,
    WindowYMin = 6,
    WindowYMax = 8,
    BufferId = 10,
    Microseconds = 12,
    Seconds = 16,
    AdcStart = 30
  };

  enum DataBlockError {
    IdMismatch = 0x1,
    UnphysicalXMin = 0x10,
    UnphysicalXMax = 0x20,
    UnphysicalYMin = 0x40,
    UnphysicalYMax = 0x80
  };

  enum RoiError {
    WrongXMin = 0x01,
    WrongXMax = 0x02,
    WrongYMin = 0x04,
    WrongYMax = 0x08
  };

  /*!\brief The raw data block as read from the FPGA.
   */ 
  unsigned char *m_rawBuffer;

  /* \brief Size of the useful part of the buffer in bytes.
   */
  unsigned int m_size;
  
  /*! \brief Vector of the event offsets, relative to the start of the buffer.
   */
  std::vector<unsigned int> m_offsetVec;

  /*! \brief Data block error summary (0 if the buffer is good).
   */
  unsigned int m_errorSummary;
  
  /*! \brief Flag for window mode events.
   */
  const bool m_isWindowed;
   
  /*
   */
  unsigned int dataWord(unsigned int offset) const;
  


  /*
   */
  unsigned int dataWord(unsigned int event, unsigned int offset) const; 
};

#endif //PDATABLOCK_H
