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
  pDataBlock(unsigned char *rawDataBlock, int bufferSize);
  ~pDataBlock() {;}

  // These are used for the UDP socket. I am not sure they belong here.
  inline char *getCharDataBlock() {return (char*)m_rawBuffer;}
  inline char *getCharDataBlock(int index)
    {return (char*)(m_rawBuffer + index);}

  inline unsigned char rawBuffer() const {return *m_rawBuffer;}
  inline std::vector<int> eventBounds() const {return m_eventBounds;}
  inline unsigned int errorSummary() const {return m_errorSummary;}

  unsigned int size() const {return m_bufferSize;}
  unsigned int numEvents() const {return m_eventBounds.size();}
  unsigned int offset(unsigned int event) const {return m_eventBounds[event];}
  unsigned int header(unsigned int event) const {return dataWord(event, 0);}
  unsigned int xmin(unsigned int event) const {return dataWord(event, 2);}
  unsigned int xmax(unsigned int event) const {return dataWord(event, 4);}
  unsigned int ymin(unsigned int event) const {return dataWord(event, 6);}
  unsigned int ymax(unsigned int event) const {return dataWord(event, 8);}
  unsigned int bufferId(unsigned int event) const {return dataWord(event, 10);}
  unsigned int numPixels(unsigned int event) const;
  double timestamp(unsigned int event) const;
  double averageEventRate() const;

  std::ostream& fillStream(std::ostream& os) const;
  friend std::ostream& operator<<(std::ostream& os, const pDataBlock& block)
    {return block.fillStream(os);}

 private:

  /*! \brief The raw data block as read from the FPGA.
  */  
  unsigned char *m_rawBuffer;

  /*
   */
  unsigned int m_bufferSize;
  
  /*! \brief Vector containing the raw data array index values at which
    the offsets of the various events are located.
  */
  std::vector<int> m_eventBounds;

  /* 
   */
  unsigned int m_errorSummary;

  /*
   */
  unsigned int dataWord(unsigned int event, unsigned int offset) const; 
};

#endif
