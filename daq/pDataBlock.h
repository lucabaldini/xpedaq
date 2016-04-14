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

#ifndef PDATABLOCK_H
#define PDATABLOCK_H

#include <vector>
#include <iostream>
#include <sstream>
#include "pUsbController.h"

#define _BITESWAP_(x1, x2) (((x2 & 0xff) << 8) | (x1 & 0xff))

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

  /*! \brief Constructor used in full-frame mode.*/
  pDataBlock(unsigned char *rawDataBlock);
  /*! \brief Constructor used in windowed mode.*/
  pDataBlock(unsigned char *rawDataBlock, int bufferSize);
  /*! \brief Do nothing destructor.*/
  ~pDataBlock()
    {;}
  /*! \brief Return the whole raw data block.*/
  inline unsigned char *getRawDataBlock()
    {return m_rawDataBlock;}
  /*! \brief Return the raw data block element at a given position.*/
  inline unsigned char getRawDataBlock(int index)
    {return m_rawDataBlock[index];}
  /*! \brief Return the whole raw data block, casted to char*.*/
  inline char *getCharDataBlock()
    {return (char*)m_rawDataBlock;}
  /*! \brief Return an element of the raw data block, casted to char*.*/
  inline char *getCharDataBlock(int index)
    {return (char*)(m_rawDataBlock + index);}
  /*! \brief Return the vector containing the headers position.*/
  inline std::vector<int> getHeaderPositions()
    {return m_headerPositions;}
  /*! \brief Return the block size (0 if the block is empty).*/
  int getSize();
  /*! \brief Return the number of events into the data block (0 if the block is empty).*/
  int getNumEvents();
  /*! \brief Return a single 16 bit data word from the data block.*/
  int getDataWord(int eventId, int offset);
  /*! \brief Return the header for a given event.*/
  int getHeader(int eventId)
    {return getDataWord(eventId, 0);}
  /*! \brief Return the window x min for a given event.*/
  int getWindowMinX(int eventId)
    {return getDataWord(eventId, 16);}
  /*! \brief Return the window x max for a given event.*/
  int getWindowMaxX(int eventId)
    {return getDataWord(eventId, 32);}
  /*! \brief Return the window y min for a given event.*/
  int getWindowMinY(int eventId)
    {return getDataWord(eventId, 48);}
  /*! \brief Return the window y max for a given event.*/
  int getWindowMaxY(int eventId)
    {return getDataWord(eventId, 64);}
  /*! \brief Return the event number for a given event.*/
  int getEventNumber(int eventId)
    {return getDataWord(eventId, 80);}
  /*! \brief Return the timestamp (in seconds) of a given event in the data block.*/
  double getTimestamp(int eventId);
  /*! \brief Return the average event rate of the data block.*/
  double getAverageEventRate();

 private:

  /*! \brief The raw data block as read from the FPGA.  

    This is stored, no matter what the acquisition mode is. In the most
    general case it should look like:
    \verbatim
    | HEADER-DATA | HEADER-DATA | ... | HEADER-DATA | HEADER-TRUNC_DATA
    |    event    |    event    | ... |    event    |      garbage
    \endverbatim*/  
  unsigned char *m_rawDataBlock;
  /*! \brief STL vector containing the raw data array index values at which
    the headers of the various events are located.

    This is calculated in the constructor and stored along with the raw data
    block. The first element points to the beginning of the first event
    (and should be zero under normal conditions) while the last element points
    to the end of the last event. If the data block contains n events, there
    should be n+1 elements in the \ref m_headerPositions member.*/
  std::vector<int> m_headerPositions;
};

#endif
