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

#ifndef PRUNCONTROLLER_H
#define PRUNCONTROLLER_H

#include <iostream>
#include <QTimer>

#include "pMainWindow.h"
#include "pFiniteStateMachine.h"
#include "pDataCollector.h"
#include "pUsbController.h"
#include "pXpolFpga.h"
#include "xpollog.h"
#include "xpolio.h"
#include "xpolenv.h"

class pMainWindow;


/*! \brief The run controller (responsible for the data acquisition).*/

class pRunController : public pFiniteStateMachine
{
  
  Q_OBJECT
  
 public:
  
  pRunController(pMainWindow *parentWindow,
		 int maxElapsedSeconds = 604800,
		 int maxAcquiredEvents = 100000000,
		 int maxAcquiredDataBlocks = 50000000,
		 int dataAcquisitionTimerTimeout = 1000);
  ~pRunController() {;}
  unsigned long connectToQuickUsb();
  void init();
  const inline pDataCollector *getDataCollector() {return m_dataCollector;}
  const inline pUsbController *getUsbController() {return m_usbController;}
  const inline pXpolFpga *getXpolFpga() {return m_xpolFpga;}
  inline int getNumAcquiredDataBlocks()
    {return m_dataCollector->getNumAcquiredDataBlocks();}
  inline int getNumAcquiredEvents()
    {return m_dataCollector->getNumAcquiredEvents();}
  double getAverageDaqEventRate();
  double getInstantDaqEventRate();
  double getInstantFpgaEventRate()
    {return m_dataCollector->getInstantFpgaEventRate();}
  void setOutputFilePath(std::string outputFilePath)
    {m_outputFilePath = outputFilePath;}
  void setMaxElapsedSeconds(int maxElapsedSeconds)
    {m_maxElapsedSeconds = maxElapsedSeconds;}
  void setMaxAcquiredDataBlocks(int maxAcquiredDataBlocks)
    {m_maxAcquiredDataBlocks = maxAcquiredDataBlocks;}
  void setMaxAcquiredEvents(int maxAcquiredEvents)
    {m_maxAcquiredEvents = maxAcquiredEvents;}
  inline void setBatch(bool batch = true)
    {m_closeParentOnStop = batch;}

 public slots:

  // This should go away. 
  void stopParent();

 signals:
  
  void quickusbError(unsigned long errorCode);
  void runIdChanged(int runId);
  void elapsedSecondsChanged(int elapsedSeconds);
  void numAcquiredDataBlocksChanged(int numDataBlocks);
  void numAcquiredEventsChanged(int numEvents);
  void averageEventRateChanged(double rate);
  void instantEventRateChanged(double rate);
  void instantFpgaEventRateChanged(double rate);

 protected:
  
  void fsmSetup();
  void fsmTeardown();
  void fsmStartRun();
  void fsmStopRun();
  void fsmPause();
  void fsmResume();
  void fsmStop();

 private:

  // Candidates for removal.
  pMainWindow *m_parentWindow;
  bool m_closeParentOnStop;

  // Sensible stuff.
  int m_maxElapsedSeconds;
  int m_maxAcquiredEvents;
  int m_maxAcquiredDataBlocks;
  pUsbController *m_usbController;
  pDataCollector *m_dataCollector;
  pXpolFpga *m_xpolFpga;
  std::string m_runIdCfgFilePath;
  std::string m_outputFilePath;
  int m_runId;
  QTimer *m_dataAcquisitionTimer;
  int m_dataAcquisitionTimerTimeout;
  int m_elapsedSeconds;
  int m_lastNumAcquiredEvents;
  int readRunId();
  void writeRunId();
  void setRunId(int runId);
  void incrementRunId();

 private slots:
   
  void updateTimer();
  void resetTimer();
};

#endif //PRUNCONTROLLER_H
