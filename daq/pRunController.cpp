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

#include "pRunController.h"

/*!
  The USB ports are scanned searching for QuickUSB module(s) connected;
  effectively \ref createUsbModules() is the first method called. If a
  QuickUsb module is found the \ref m_usbController is created and,
  immediately after, \ref m_xpolFpga \ref m_dataCollector are also created.
  Otherwise all the widgets which potentially interact with the hardware are
  disabled; the rest may be useful for the debug.
  The connections are set up.

  \todo Think about wheter it could be handy not to disable any widget but
  rather create a <i>dummy</i> \ref pRunController::m_usbController in case
  the hardware is not available. It may require some work, but it could be
  worth it in terms of debugging capability.

  \todo Move the connections setup in a separate function.
 */

pRunController::pRunController(pMainWindow *parentWindow,
			       int maxElapsedSeconds,
			       int maxAcquiredEvents,
			       int maxAcquiredDataBlocks,
			       int dataAcquisitionTimerTimeout)
  :m_parentWindow(parentWindow),
   m_maxElapsedSeconds(maxElapsedSeconds),
   m_maxAcquiredEvents(maxAcquiredEvents),
   m_maxAcquiredDataBlocks(maxAcquiredDataBlocks),
   m_dataAcquisitionTimerTimeout(dataAcquisitionTimerTimeout)
{
  m_closeParentOnStop = false;
  m_outputFilePath = xpolenv::kNullPath;
  // Create the runId.cfg file, if needed.
  QString cfgFilePath = QString(std::getenv("XPEDAQ_ROOT")) +
    QDir::separator() + "xpedaq" + QDir::separator() + "config" +
    QDir::separator() + "runId.cfg";
  if (!QFile(cfgFilePath).exists()) {
    *xpollog::kInfo << "Creating " << cfgFilePath.toStdString() <<
      "..." << endline;
    QFile(cfgFilePath + ".sample").copy(cfgFilePath);
  }
  m_runIdCfgFilePath = cfgFilePath.toStdString();
}

unsigned long pRunController::connectToQuickUsb()
{
  *xpollog::kInfo << "Searching for USB module(s)... " << endline;
  unsigned long errorCode = 0;
  char usbDeviceString[512];
  // Execute the QuickUsb::QuickUsbFindModules and see what's connected.
  // If this finds something, we go ahead and parse the return string.
  if (QuickUsbFindModules(usbDeviceString, 512)) {
    int numUsbDevices = 0;
    char usbDeviceName[20];
    char *usbDeviceStringPtr = usbDeviceString;
    while (*usbDeviceStringPtr != '\0') {
      *xpollog::kDebug << "Found " << usbDeviceStringPtr << "." << endline;
      strncpy(usbDeviceName, usbDeviceStringPtr, 20);
      usbDeviceStringPtr += strlen(usbDeviceStringPtr);
      numUsbDevices ++;
    }
    // We're not equipped to handle more than one device, yet.
    if (numUsbDevices > 1) {
      *xpollog::kWarning << numUsbDevices << " modules found." << endline;
    }
    // Create the pUsbController object.
    m_usbController = new pUsbController(usbDeviceName);
  }
  // And this is the case where there are no QuickUsb interfaces attached:
  // we parse the last error code and emit a quickusbError(errorCode)
  // signal.
  else {
    QuickUsbGetLastError(&errorCode);
    if (errorCode > 0) {
      *xpollog::kError << "Error " << errorCode << ". "
		       << pUsbController::getErrorDescription(errorCode)
		       << endline;
    }
    *xpollog::kError << "No USB module found." << endline;
    emit quickusbError(errorCode);
  }
  // Irrespectively, we create the remaining objects.
  // Mind that if there's no USB connection, m_usbController is a null
  // pointer. Do we have to do anything about it?
  m_xpolFpga = new pXpolFpga(m_usbController);
  m_dataCollector = new pDataCollector(m_usbController);
  m_dataAcquisitionTimer = new QTimer();
  connect(m_dataAcquisitionTimer, SIGNAL(timeout()), this, SLOT(updateTimer()));
  connect(m_dataCollector, SIGNAL(readoutErrorDetected(int)), this,
	  SLOT(stopParent()));
  return errorCode;
}

/*!
  Called once and forever when the parent GUI window is instantiated. The
  setReset() method of the \ref pFiniteStateMachine class is called, the run
  number is read from its text file and set and the data collector is reset.
  The main purpose of the function is to have the correct information on the
  data display of the main window when the window itself is created.

  \todo The \ref pRunController::resetTimer() method is called in the
  \ref pRunController::fsmStartRun method so that it's effectively done twice.
  The same holds for the reset of the data collector. Look into that more
  carefully.

  \todo Why not move the whole block into the constructor? Is there any
  particular reason for calling it from the main window?
 */

void pRunController::init()
{
  *xpollog::kDebug << "Initializing the run controller... " << endline;
  pFiniteStateMachine::setReset();
  resetTimer();
  setRunId(readRunId());
  m_dataCollector->reset();
  *xpollog::kDebug << "Run controller initialized." << endline;
}

/*!
  The number of elapsed seconds is incremented and the necessary signals
  (connected with the parent window, if any) are emitted. In case one
  of the stop criteria (on the elapsed time or the number of acquired data
  blocks or event) is satisfied, the acquisition is stopped.

  The slot is connected to the timeout() method of the 
  \ref m_dataAcquisitionTimer QTimer member which is created in the
  constructor, started in the \ref fsmStartRun() method and stopped in the
  \ref fsmStopRun() method.
  Effectively the method is executed once per second (or whatever, depending
  on the QTimer settings) all the time while the data acquisition is running.
 
  Also note that the number of acquired events or data blocks is also
  checked once per second, at the moment, so that typically more data blocks
  and events of those requested will be acquired.
 
  \todo Fix the m_closeParent thing (eventually will go away).
  
  \todo Change the name of the method to updateElapsedSeconds().
 */

void pRunController::updateTimer()
{
  m_elapsedSeconds ++;
  emit elapsedSecondsChanged(m_elapsedSeconds);
  emit numAcquiredDataBlocksChanged(getNumAcquiredDataBlocks());
  emit numAcquiredEventsChanged(getNumAcquiredEvents());
  emit averageEventRateChanged(getAverageDaqEventRate());
  emit instantEventRateChanged(getInstantDaqEventRate());
  emit instantFpgaEventRateChanged(0.0);
  m_lastNumAcquiredEvents = getNumAcquiredEvents();
  if ((m_elapsedSeconds > m_maxElapsedSeconds)               ||
      (getNumAcquiredDataBlocks() > m_maxAcquiredDataBlocks) ||
      (getNumAcquiredEvents() > m_maxAcquiredEvents)) {
    stopParent();
  }
}

void pRunController::stopParent()
{
  m_parentWindow->stop();
  while (m_dataCollector->isRunning())
    {};
  if (m_closeParentOnStop) {
    *xpollog::kInfo << "Closing parent window..." << endline;
    m_parentWindow->close();
  }  
}

/*!
  Set the \ref m_elapsedSeconds member to zero and emit an
  \ref elapsedSecondsChanged signal.

  Called in the \ref fsmStartRun method.

  \todo Change the name to resetElapsedSeconds().

  \todo Check whether this should really be a slot, since it does not seem to
  be connected to anything.
 */

void pRunController::resetTimer()
{
  m_lastNumAcquiredEvents = 0;
  m_elapsedSeconds = 0;
  emit elapsedSecondsChanged(m_elapsedSeconds);
  emit numAcquiredDataBlocksChanged(0);
  emit numAcquiredEventsChanged(0);
  emit averageEventRateChanged(0.0);
  emit instantEventRateChanged(0.0);
  emit instantFpgaEventRateChanged(0.0);
}

/*!
  Return the ratio between the number of events acquired by the
  data collector and the number of elapsed seconds.
 */

double pRunController::getAverageDaqEventRate()
{
  if (m_elapsedSeconds == 0) {
    return 0.0;
  } else {
    return getNumAcquiredEvents()/(double)m_elapsedSeconds; 
  }
}

double pRunController::getInstantDaqEventRate()
{
  return 1000.0*(getNumAcquiredEvents() - m_lastNumAcquiredEvents)/
    m_dataAcquisitionTimerTimeout;
}

/*!
  \return The run number.
*/

int pRunController::readRunId()
{
  *xpollog::kDebug << "Reading run Id from " << m_runIdCfgFilePath <<
    "..." << endline;
  return xpolio::kIOManager->getInteger(m_runIdCfgFilePath);
}

void pRunController::writeRunId()
{
  *xpollog::kDebug << "Writing run Id to " << m_runIdCfgFilePath <<
    "... " << endline;
  xpolio::kIOManager->put(m_runIdCfgFilePath, m_runId);
}

/*!
  After it has been incremented, the new run number is immediately written
  to file.
 */

void pRunController::incrementRunId()
{
  setRunId(m_runId + 1);
  writeRunId();
}

/*!
  This changes the \ref m_runId member and emit a \ref runIdChanged signal
  so that the rest of the world knows; it <i>does not</i> write the runId
  to file.
  This is because this method is called both in the \ref init() method (where
  the run number does not need to be written to file) and in the
  \ref incrementRunId() method.

  \param runId
  The run number.
 */

void pRunController::setRunId(int runId)
{
  m_runId = runId;
  emit runIdChanged(m_runId); 
}

void pRunController::fsmSetup()
{}

/*!
  This is executed at the beginning of each data acquisition run (which is
  essentially each time the start button is pressed on the transport bar of the
  main window or right after the run controller is created, if the program
  is run in batch mode).
  
  Here is a list of the operations which are executed:
  \li The run number is incremented (\ref incrementRunId()).
  \li The path to the output data file {\ref m_outputFilePath} is generated
  from the run number, unless it has been explicitely set before (and therefore
  is different from {\ref xpolenv::kNullPath}).
  \li The data collector \ref m_dataCollector is reset.
  \li The data collector \ref m_dataCollector and the FPGA controller
  \ref m_xpolFpga are set up according to the current configuration and
  user preferences.
  \li The data acquisition timer is reset (\ref resetTimer()) and restarted.

  \todo Create the path to the output data file in a separate method, to be
  used by the "dummy" run controller when running in test mode.

  \todo Create a m_configuration member and implement a setConfiguration()
  method with two different prototypes, accepting a *pDetectorConfiguration
  and a std::string (file path) as parameters, respectively, to be used when
  the configuratio is retrieved from the GUI or from a file (batch mode).
*/

void pRunController::fsmStartRun()
{
  *xpollog::kInfo << "Starting run controller..." << endline;
  incrementRunId();
  // Create the output folder, if it does not exist.
  QString outputFolder = m_parentWindow->currentOutputFolder();
  if (!QDir(outputFolder).exists()) {
    *xpollog::kInfo << "Creating " << outputFolder.toStdString() <<
      "..." << endline;
    QDir().mkpath(outputFolder);
  }
  // Set the output path for the logger.
  QString logFilePath = outputFolder + QDir::separator() +
    m_parentWindow->currentDataIdentifier() + ".log";
  std::cout << logFilePath.toStdString() << std::endl;
  xpollog::kLogger->setLogFilePath(logFilePath.toStdString());
  xpollog::kLogger->enableLogFile(true);
  // Save the run info.
  m_parentWindow->saveRunInfo(outputFolder);
  // Why do we need this?
  if (m_outputFilePath == xpolenv::kNullPath) {
    m_outputFilePath = m_parentWindow->currentDataFilePath().toStdString();
  }
  *xpollog::kDebug << "Output file path: " << m_outputFilePath << endline;
  m_dataCollector->reset();
  if (m_usbController->IsOpened()) {
    m_xpolFpga->setup(m_parentWindow->getConfiguration());
    m_dataCollector->setup(m_outputFilePath,
			   m_parentWindow->getUserPreferences(),
			   m_parentWindow->getConfiguration());
    m_dataCollector->start();
  } else {
    *xpollog::kError << "The USB device is not open." << endline;
  }
  resetTimer();
  m_dataAcquisitionTimer->start(m_dataAcquisitionTimerTimeout);
  *xpollog::kInfo << "Run controller started." << endline;
}

/*!
  This is executed at the end of each data acquisition run (which is
  essentially each time the stop button is pressed on the transport bar of the
  main window or right after the acquisition is done, if the program is run
  in batch mode).
  
  Here is a list of the operations which are executed:
  \li The data collector \ref m_dataCollector is stopped.
  \li The data acquisition timer \ref m_dataAcquisitionTimer is stopped.
  \li The path to the output file \ref m_outputFilePath is set to
  \ref xpolenv::kNullPath. This is done in order to prevent the run controller
  from overwriting over and over again the same file when started multiple]
  times from the GUI.
*/

void pRunController::fsmStopRun()
{
  *xpollog::kInfo << "Stopping run controller..." << endline;
  m_dataCollector->stop();
  m_dataAcquisitionTimer->stop();
  m_outputFilePath = xpolenv::kNullPath;
  *xpollog::kDebug << "Output file path: " << m_outputFilePath << endline;
  *xpollog::kInfo << "Run controller stopped." << endline;
  *xpollog::kInfo << getNumAcquiredEvents() << " events (" <<
    getNumAcquiredDataBlocks() << " data blocks) acquired in "<<
    m_elapsedSeconds << " seconds."<< endline;
  xpollog::kLogger->enableLogFile(false);
}

void pRunController::fsmPause()
{
  *xpollog::kInfo << "Run controller paused." << endline;
  m_dataCollector->stop();
}

void pRunController::fsmResume()
{
  *xpollog::kInfo << "Run controller restarted." << endline;
  if (m_usbController->IsOpened()) {
    m_dataCollector->start();
  } else {
    *xpollog::kError << "The USB device is not open." << endline;
  }
}

void pRunController::fsmStop()
{
  fsmStopRun();
}

void pRunController::fsmTeardown()
{}
