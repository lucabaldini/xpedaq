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

#ifndef PRUNCONTROLLER_H
#define PRUNCONTROLLER_H

#include <iostream>
#include <QTimer>
#include "pMainWindow.h"
#include "pFiniteStateMachine.h"
#include "pDataCollector.h"
#include "pUsbController.h"
#include "pXpolFpga.h"
#include "pHeaderFile.h"
#include "xpollog.h"
#include "xpolio.h"
#include "xpolenv.h"

class pMainWindow;


/*! \brief The run controller (responsible for the data acquisition).*/

class pRunController : public pFiniteStateMachine
{
  
  Q_OBJECT

 public:
  
  /*! \brief Basic constructor.*/
  pRunController(pMainWindow *parentWindow,
		 int maxElapsedSeconds = 604800,
		 int maxAcquiredEvents = 100000000,
		 int maxAcquiredDataBlocks = 50000000,
		 int dataAcquisitionTimerTimeout = 1000);
  /*! \brief Basic destructor.*/
  ~pRunController()
    {;}
  unsigned long connectToQuickUsb();
  /*! \brief Return the member \ref pRunController::m_dataCollector.*/
  const inline pDataCollector *getDataCollector()
    {return m_dataCollector;}
  /*! \brief Return the member \ref pRunController::m_usbController.*/
  const inline pUsbController *getUsbController()
    {return m_usbController;}
  /*! \brief Return the member \ref pRunController::m_xpolFpga.*/
  pXpolFpga *getXpolFpga()
    {return m_xpolFpga;}
  /*! \brief Return the number of data blocks acquired by the underlying data
    collector (\ref pRunController::m_dataCollector).*/
  inline int getNumAcquiredDataBlocks()
    {return m_dataCollector->getNumAcquiredDataBlocks();}
  /*! \brief Return the number of events acquired by the underlying data
    collector (\ref pRunController::m_dataCollector).*/
  inline int getNumAcquiredEvents()
    {return m_dataCollector->getNumAcquiredEvents();}
  /*! \brief Return the average DAQ event rate.*/
  double getAverageDaqEventRate();
  /*! \brief Return the instant DAQ event rate.*/
  double getInstantDaqEventRate();
  /*! \brief Return the instant FPGA event rate.*/
  double getInstantFpgaEventRate()
    {return m_dataCollector->getInstantFpgaEventRate();}
  /*! \brief Initialize the run controller.*/
  void init();
  /*! \brief Set the path to the output data binary file
    (\ref pRunController::m_outputFilePath).*/
  void setOutputFilePath(std::string outputFilePath)
    {m_outputFilePath = outputFilePath;}
  /*! \brief Set the path to the input (optional) header file
    (\ref pRunController::m_headerFilePath).*/
  void setHeaderFilePath(std::string headerFilePath)
    {m_headerFilePath = headerFilePath;}
  /*! \brief Set the number of seconds after which the acquisition is
    automatically stopped (\ref pRunController::m_maxElapsedSeconds).*/
  void setMaxElapsedSeconds(int maxElapsedSeconds)
    {m_maxElapsedSeconds = maxElapsedSeconds;}
  /*! \brief Set the (approximate) number of acquired data blocks after which
    the acquisition is automatically stopped
    (\ref pRunController::m_maxAcquiredDataBlocks).*/
  void setMaxAcquiredDataBlocks(int maxAcquiredDataBlocks)
    {m_maxAcquiredDataBlocks = maxAcquiredDataBlocks;}
  /*! \brief Set the (approximate) number of acquired events after which the
    acquisition is automatically stopped
    (\ref pRunController::m_maxAcquiredEvents).*/
  void setMaxAcquiredEvents(int maxAcquiredEvents)
    {m_maxAcquiredEvents = maxAcquiredEvents;}
  /*! \brief Set the run controller in batch mode (essentially close the parent
    window in stop).
    \todo This is going to disappear as when the run controller is supposed
    to run in bach mode, the GUI window shouldn't be created at all.*/
  inline void setBatch(bool batch = true)
    {m_closeParentOnStop = batch;}

 public slots:
  void stopParent();
  //void prepareStartRun();

 signals:
  void usbConnectionError(unsigned long);
  /*! \brief Notify that the runId has changed.*/
  void runIdChanged(int runId);
  /*! \brief Notify that the number of elapsed acquisition seconds is
    changed.*/
  void elapsedSecondsChanged(int elapsedSeconds);
  /*! \brief Notify that the number of acquired data blocks is changed.*/
  void numAcquiredDataBlocksChanged(int numDataBlocks);
  /*! \brief Notify that the number of acquired events is changed.*/
  void numAcquiredEventsChanged(int numEvents);
  /*! \brief Notify that the average DAQ event rate has been sampled.*/
  void averageEventRateChanged(double rate);
  /*! \brief Notify that the instant DAQ event rate has been sampled.*/
  void instantEventRateChanged(double rate);
  /*! \brief Notify that the instant FPGA event rate has been sampled.*/
  void instantFpgaEventRateChanged(double rate);

 protected:
  /*! \brief Function called when passing from RESET to STOPPED.*/
  virtual void fsm_setup();
  /*! \brief Function called when passing from STOPPED to RESET.*/
  virtual void fsm_teardown();
  /*! \brief Function called when passing from STOPPED to RUNNING.*/
  virtual void fsm_startRun();
  /*! \brief Function called when passing from RUNNING to STOPPED.*/
  virtual void fsm_stopRun();
  /*! \brief Function called when passing from RUNNING to PAUSED.*/
  virtual void fsm_pause();
  /*! \brief Function called when passing from PAUSED to RUNNING.*/
  virtual void fsm_resume();
  /*! \brief Function called when passing from PAUSED to STOPPED.*/
  virtual void fsm_stop();

 private:
  /*! \brief The parent GUI window (if any) the run controller belongs to.*/
  pMainWindow *m_parentWindow;
  /*! \brief The number of seconds after which the data acquisition is
    automatically stopped.*/
  int m_maxElapsedSeconds;
  /*! \brief The (approximate) number of acquired events after which the data
    acquisition is automatically stopped.*/
  int m_maxAcquiredEvents;
  /*! \brief The (approximate) number of acquired data blocks after which the
    data acquisition is automatically stopped.*/
  int m_maxAcquiredDataBlocks;
  /*! \brief Boolean flag telling whether the run controller has to close the
    parent window upon termination of the data collection.*/
  bool m_closeParentOnStop;
  /*! \brief The base USB controller object.*/
  pUsbController *m_usbController;
  /*! \brief The data collector object.*/
  pDataCollector *m_dataCollector;
  /*! \brief The base xpol FPGA object.*/
  pXpolFpga *m_xpolFpga;
  /*! \brief The path to the file containing the run number of the last
    acquisition.*/
  std::string m_runIdCfgFilePath;
  /*! \brief The path to the output data binary file. Defaults to
    {\ref xpolenv::kNullPath}.*/
  std::string m_outputFilePath;
  /*! \brief The path to the (optional) header file which, if passed, is
    prepended in the data file (\ref pRunController::m_outputFilePath) to the
    actual data. Defaults to {\ref xpolenv::kNullPath}.*/
  std::string m_headerFilePath;
  /*! \brief The run id of the current (if the run controller is running) or
    the last (if it's not) acquisition.*/
  int m_runId;
  /*! \brief A QTimer triggering the signals to the outside world (i.e. the
    update signals for the parent GUI window, if any).*/
  QTimer *m_dataAcquisitionTimer;
  /*! \brief The timeout for \ref pRunController::m_dataAcquisitionTimer (in msec).*/
  int m_dataAcquisitionTimerTimeout;
  /*! \brief The number of elapsed seconds in the current acquisition.*/
  int m_elapsedSeconds;
  /*! \brief The number of events acquired the last time the
    \ref pRunController::m_dataAcquisitionTimer has been updated. Used to calculate the
    instant event rate.*/
  int m_lastNumAcquiredEvents;
  /*! \brief Read the last run number from \ref pRunController::m_runIdCfgFilePath.*/
  int readRunId();
  /*! \brief Write the last run number to
    \ref pRunController::m_runIdCfgFilePath.*/
  void writeRunId();
  /*! \brief Set the run number to a given value.*/
  void setRunId(int runId);
  /*! \brief Increment the run number by one unit.*/
  void incrementRunId();
  /*! \brief Write the header information read from
    \ref pRunController::m_headerFilePath to the output data binary file
    (\ref pRunController::m_outputFilePath).*/
  void writeHeader();

 private slots:
  /*! \brief Update the elapsed time (emitting the related signals) and stop
    the acquisition, if necessary.*/
  void updateTimer();
  /*! \brief Reset the elapsed time.*/
  void resetTimer();
};

#endif
