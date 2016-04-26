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

#ifndef PDATAFIFO_H
#define PDATAFIFO_H

#include <iostream>
#include <vector>
#include <QObject>
#include "xpolio.h"
#include "xpollog.h"
#include "pDataBlock.h"
#include "pUdpSender.h"
#include "pUserPreferences.h"
#include <iostream>


/*! \brief Class implementing a FIFO containing data blocks.

  This constitutes one of the most important objects in terms of data IO,
  being responsible for:
  \li writing data to file;
  \li multicasting data to monitor applications (through the methods of the
  base class pUdpSender).

  The main underlying object is a STL vector of pointers to pDataBlok objects.
  Each time a data block is read out, the data collector puts it into the data
  FIFO; the \ref pDataFIFO::flush() method can be called to write the content
  of the data block vector to file and empty the vector itself.
 */

class pDataFIFO : pUdpSender
{

 public:
  pDataFIFO(std::string outputFilePath, pUserPreferences *preferences);
  ~pDataFIFO() {;}
  void fill(pDataBlock *dataBlock);
  inline int getNumDataBlocks() const {return m_buffer.size();}
  inline int getNumEvents() const {return m_numEvents;}
  inline int getSize() const {return m_size;}
  inline bool isEmpty() const {return m_buffer.empty();}
  inline int getNumAcquiredDataBlocks() const {return m_acquiredDataBlocks;}
  inline int getNumAcquiredEvents() const {return m_acquiredEvents;}
  inline double getLastDataBlockAverageEventRate()
    {return m_buffer.back()->averageEventRate();}
  void flush();
  void setStartSeconds(unsigned int startSeconds);

 private:

  /*! \brief Basic STL container for the data block.
   */
  std::vector<pDataBlock *> m_buffer;
  
  /*! \brief Iterator over the STL vector. Used while flushing for looping.
   */
  std::vector<pDataBlock *>::iterator m_iterator;
  
  /*! \brief The total number of events <i>currently</i> into the FIFO.
   */
  int m_numEvents;
  
  /*! \brief The total <i>current</i> size of the FIFO.
   */
  int m_size;
  
  /*! \brief The number of blocks acquired since the fifo has been created.
   */
  int m_acquiredDataBlocks;
  
  /*! \brief The number of events acquired since the fifo has been created.
   */
  int m_acquiredEvents;
  
  /*! \brief The path to the output data file.
   */
  std::string m_outputFilePath;
  
  /*! \brief Flag telling whether the multicast is enabled or not.
   */
  bool m_multicastEnabled;
};

#endif
