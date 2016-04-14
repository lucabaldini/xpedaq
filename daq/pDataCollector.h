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

#ifndef PDATACOLLECTOR_H
#define PDATACOLLECTOR_H

#include <iostream>
#include <QThread>
#include "xpollog.h"
#include "pUsbController.h"
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

  /*! \brief Basic constructor.*/
  pDataCollector(pUsbController *usbController);
  /*! \brief Destructor.*/
  ~pDataCollector();
  /*! \brief Reset the data collector.*/
  void reset();
  /*! \brief Setup the data collector.*/
  void setup(std::string outputFilePath, pUserPreferences *preferences,
	     pDetectorConfiguration *configuration);
  /*! \brief Return the number of acquired data blocks.*/
  inline int getNumAcquiredDataBlocks()
    {return m_dataFIFO->getNumAcquiredDataBlocks();}
  /*! \brief Return the number of acquired Events.*/
  inline int getNumAcquiredEvents()
    {return m_dataFIFO->getNumAcquiredEvents();}
  inline double getInstantFpgaEventRate()
    {return m_dataFIFO->getLastDataBlockAverageEventRate();}

 signals:
  /*! \brief Notify that a readout error has been detected. */
  void readoutErrorDetected(int errorCode);

 public slots:
  /*! \brief Stop the data collector.*/
  void stop();

 protected:
  /*! \brief Overloaded QThread method called when the QThread::start()
   method is called.*/
  void run();

 private:

  /*! \brief Flag indicating whether the data collector is running or not.*/
  bool m_running;
  /*! \brief Pointer to the USB controller.*/
  pUsbController *m_usbController;
  /*! \brief Pointer to the data FIFO.*/
  pDataFIFO *m_dataFIFO;
  /*! \brief Path to the output file.*/ 
  std::string m_outputFilePath;
  /*! \brief User preferences.*/
  pUserPreferences *m_userPreferences;
  /*! \brief Detector configuration.*/
  pDetectorConfiguration *m_detectorConfiguration;
  /*! \brief Convenience member telling whether the data radout is supposed
    to be in full frame.*/
  bool m_fullFrame;
};

#endif
