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

#ifndef PDATACOLLECTOR_H
#define PDATACOLLECTOR_H

#include <iostream>
#include <ctime>

#include <QThread>

#include "xpollog.h"
#include "pDataFIFO.h"
#include "pUserPreferences.h"
#include "pDetectorConfiguration.h"


/*! \brief Class implementing the data collection loop.
  
  The loop is executed on a different thread, as the class inherits from the
  Qt QThread class.

  \todo Qt allows to spawn threads with adjustable priority. We should
  investigate that; see for example if CriticalTimingApplication is well
  suited for our purposes.

  \todo Think about whether we want to create a class pRunInfo
  containing all the run information, containing the configuration, the
  user preferences the runId, methods for generating suitable output file
  name etc. Such an object should be passed to the pDataCollector::setup()
  function.
*/

class pDataCollector : public QThread
{

  Q_OBJECT
  
 public:

  pDataCollector(pUsbController *usbController, bool emitBlocks=false);
  ~pDataCollector() {;}
  void reset();
  void setupRun(std::string outputFilePath, long int startSeconds,
		pUserPreferences *preferences,
		pDetectorConfiguration *configuration);
  int numDataBlocks() const {return m_dataFIFO->getNumAcquiredDataBlocks();}
  int numEvents() const {return m_dataFIFO->getNumAcquiredEvents();}

 signals:
 
  void blockRead(const pDataBlock& passedDataBlock);  

 public slots:

  void stop();


  
 protected:

  void run();
  
  
 private:

  bool m_running;
  pUsbController *m_usbController;
  pDataFIFO *m_dataFIFO;
  std::string m_outputFilePath;
  pUserPreferences *m_userPreferences;
  pDetectorConfiguration *m_detectorConfiguration;
  // This is effectively part of the configuration and might go away.
  bool m_fullFrame;
  long int m_startSeconds;
  int m_numMalformedBlocks;
  bool m_emitBlocks;
  void dumpRawBuffer(unsigned char *buffer);
};

#endif //PDATACOLLECTOR_H
