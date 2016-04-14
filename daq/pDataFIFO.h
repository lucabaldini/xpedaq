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

  /*! \brief Basic constructor.*/
  pDataFIFO(std::string outputFilePath, pUserPreferences *preferences);
  /*! \brief Destructor.*/
  ~pDataFIFO();
  /*! \brief Put a data block into the FIFO.*/
  void fill(pDataBlock *dataBlock);
  /*! \brief Return the number of blocks <i>currently</i> into the FIFO.*/
  inline int getNumDataBlocks()
    {return m_buffer.size();}
  /*! \brief Return the number of events <i>currently</i> into the FIFO.*/
  inline int getNumEvents()
    {return m_numEvents;}
  /*! \brief Return the <i>current</i> total size of the FIFO.*/
  inline int getSize()
    {return m_size;}
  /*! \brief Return true if the FIFO is empty.*/
  inline bool isEmpty()
    {return m_buffer.empty();}
  /*! \brief Return the number of blocks acquired since the creation time.*/
  inline int getNumAcquiredDataBlocks()
    {return m_acquiredDataBlocks;}
  /*! \brief Return the number of events acquired since the creation tim.*/
  inline int getNumAcquiredEvents()
    {return m_acquiredEvents;}
  /*! \brief Return the average FPGA event rate in the last data block in the FIFO.*/
  inline double getLastDataBlockAverageEventRate()
    {return m_buffer.back()->getAverageEventRate();}
  /*! \brief Empty the FIFO and write to file the content.*/
  void flush();

 private:

  /*! \brief Basic STL container for the data block.*/
  std::vector<pDataBlock *> m_buffer;
  /*! \brief Iterator over the STL vector. Used while flushing for looping.*/
  std::vector<pDataBlock *>::iterator m_iterator;
  /*! \brief The total number of events <i>currently</i> into the FIFO.*/
  int m_numEvents;
  /*! \brief The total <i>current</i> size of the FIFO.*/
  int m_size;
  /*! \brief The number of blocks acquired since the fifo has been created.*/
  int m_acquiredDataBlocks;
  /*! \brief The number of events acquired since the fifo has been created.*/
  int m_acquiredEvents;
  /*! \brief The path to the output data file.*/
  std::string m_outputFilePath;
  /*! \brief Flag telling whether the multicast is enabled or not.*/
  bool m_multicastEnabled;
};

#endif
