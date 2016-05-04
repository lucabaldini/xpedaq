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



/*! Basic constructor.
 */
pRunController::pRunController(std::string configFilePath,
			       std::string preferencesFilePath)
  : m_maxSeconds(2592000),
    m_maxEvents(259200000),
    m_maxDataBlocks(2592000),
    m_startSeconds(0),
    m_stopSeconds(0),
    m_configFilePath(configFilePath),
    m_preferencesFilePath(preferencesFilePath)
{
  m_stationIdFilePath = xpedaqos::rjoin("config", "stationId.cfg");
  if (!xpedaqos::fileExists(m_stationIdFilePath)) {
    *xpollog::kError << "Could not find " << m_stationIdFilePath << endline;
    exit(1);
  }
  m_runIdFilePath = xpedaqos::rjoin("config", "runId.cfg");
  if (!xpedaqos::fileExists(m_runIdFilePath)) {
    xpedaqos::copyFile(m_runIdFilePath + ".sample", m_runIdFilePath);
  }
  if (!xpedaqos::fileExists(m_configFilePath)) {
    xpedaqos::copyFile(m_configFilePath + ".sample", m_configFilePath);
  }
  if (!xpedaqos::fileExists(m_preferencesFilePath)) {
    xpedaqos::copyFile(m_preferencesFilePath + ".sample",
		       m_preferencesFilePath);
  }
  setupRun();
  m_timer = new QTimer();
  m_timer->setInterval(1000);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(updateRunInfo()));
  m_usbController = new pUsbController();
  m_xpolFpga = new pXpolFpga(m_usbController);
  m_dataCollector = new pDataCollector(m_usbController);
}


/*!
 */
unsigned long pRunController::connectUsb()
{
  return m_usbController->connect();
}



/*! Basic initialization. This is not part of constructor because when the
  run controller is instantiated from a parent GUI we need to setup the
  connections properly before we call this.
 */
void pRunController::init()
{
  *xpollog::kDebug << "Initializing the run controller... " << endline;
  pFiniteStateMachine::setReset();
  setStationId(readStationId());
  setRunId(readRunId());
  resetRunInfo();
  m_dataCollector->reset();
  *xpollog::kDebug << "Run controller initialized." << endline;
}


/*!
 */
void pRunController::setupRun(pDetectorConfiguration *configuration,
			      pUserPreferences *preferences)
{
  *xpollog::kInfo << "Setting up run..." << endline;
  m_detectorConfiguration = configuration;
  m_userPreferences = preferences;
  // The following two lines are duplicated and should be refactored.
  xpollog::kLogger->setTerminalLevel(m_userPreferences->loggerTerminalLevel());
  xpollog::kLogger->setDisplayLevel(m_userPreferences->loggerDisplayLevel());
}


/*!
 */
void pRunController::setupRun(std::string configFilePath,
			      std::string preferencesFilePath)
{ 
  m_detectorConfiguration = new pDetectorConfiguration(configFilePath);
  m_userPreferences = new pUserPreferences(preferencesFilePath);
  xpollog::kLogger->setTerminalLevel(m_userPreferences->loggerTerminalLevel());
  xpollog::kLogger->setDisplayLevel(m_userPreferences->loggerDisplayLevel());
}


/*!
 */
void pRunController::setupRun()
{
  setupRun(m_configFilePath, m_preferencesFilePath);
}


/*!
 */
int pRunController::readStationId() const
{
  *xpollog::kDebug << "Reading station ID from " << m_stationIdFilePath <<
    "...";
  int stationId = xpolio::kIOManager->getInteger(m_stationIdFilePath);
  *xpollog::kDebug << " (" << stationId << ")." << endline;
  return stationId;
}


/*! Set the station identifier and emit a stationIdSet() signal.
 */
void pRunController::setStationId(int stationId)
{
  m_stationId = stationId;
  emit stationIdSet(m_stationId);
}


/*! 
 */
int pRunController::readRunId() const
{
  *xpollog::kDebug << "Reading run ID from " << m_runIdFilePath <<  "...";
  int runId = xpolio::kIOManager->getInteger(m_runIdFilePath);
  *xpollog::kDebug << " (" << runId << ")." << endline;
  return runId;
}


/*!
 */
void pRunController::writeRunId() const
{
  *xpollog::kDebug << "Writing run Id to " << m_runIdFilePath <<
    "... " << endline;
  xpolio::kIOManager->put(m_runIdFilePath, m_runId);
}


/*! Set the run identifier and emit a runIdChanged() signal. Mind this is 
  not writing the new run identifier to file.
 */
void pRunController::setRunId(int runId)
{
  m_runId = runId;
  emit runIdChanged(m_runId); 
}


/*! Increment the run identifier by one unit and write the new value into
  the proper configuration file.
 */
void pRunController::incrementRunId()
{
  setRunId(m_runId + 1);
  writeRunId();
}


/*! This is read from the system time.
 */
long int pRunController::currentSeconds() const
{
  return static_cast<int> (time(NULL));
}


/*! Return the difference between the current seconds and the seconds latched
  at the start time.
*/
long int pRunController::elapsedSeconds() const
{
  return currentSeconds() - m_startSeconds;
}


/*!
 */
std::string pRunController::startDatetime() const
{
  std::string datetime(ctime(&m_startSeconds));
  datetime.erase(datetime.size() - 1);
  return datetime;
}


/*!
 */
std::string pRunController::stopDatetime() const
{
  std::string datetime(ctime(&m_stopSeconds));
  datetime.erase(datetime.size() - 1);
  return datetime;
}


/*! Return the ratio between the number of events acquired by the data
  collector and the number of elapsed seconds.
*/
double pRunController::averageEventRate() const
{
  if (elapsedSeconds() == 0) {
    return 0.0;
  } else {
    return numEvents()/(double)elapsedSeconds(); 
  }
}


/*! 
 */
double pRunController::instantEventRate() const
{
  // Need to come back to this one. This should be possible without
  // keeping track of m_lastNumAcquiredEvents.
  //return 1000.0*(getNumAcquiredEvents() - m_lastNumAcquiredEvents)/
  //  m_timer->interval();
  return 0.;
}


/*! This is connected to an underlying QTimer object that is started at the
  start run and is meant to signal the outside world that the run information
  (e.g. elapsed time and number of events collected) has changed. Most notably,
  these signals are intercepted by the main GUI to display the run info.
*/
void pRunController::updateRunInfo()
{
  emit elapsedSecondsChanged(elapsedSeconds());
  emit numDataBlocksChanged(numDataBlocks());
  emit numEventsChanged(numEvents());
  emit averageEventRateChanged(averageEventRate());
  emit instantEventRateChanged(instantEventRate());
  if ((elapsedSeconds() > m_maxSeconds)   ||
      (numDataBlocks() > m_maxDataBlocks) ||
      (numEvents() > m_maxEvents)) {
    setStopped();
  }
}


/*! Similar to the updateRunInfo() in spirit---except that here we're sending
  out a bunch of zeros.
*/
void pRunController::resetRunInfo()
{
  emit elapsedSecondsChanged(0);
  emit numDataBlocksChanged(0);
  emit numEventsChanged(0);
  emit averageEventRateChanged(0.);
  emit instantEventRateChanged(0.);
}


/*! Nothing to do here.
 */
void pRunController::fsmSetup()
{

}


/*!
 */
void pRunController::fsmStartRun()
{
  *xpollog::kInfo << "Starting run controller..." << endline;
  incrementRunId();
  // Create the output folder.
  std::string outputFolder = outputFolderPath();
  if (!xpedaqos::folderExists(outputFolder)) {
    xpedaqos::mkdir(outputFolder);
  }
  *xpollog::kDebug << "Output file set to " << dataFilePath() << "." << endline;
  // Set the log file path.
  std::string logFile = logFilePath();
  *xpollog::kInfo << "Redirecting logger to " << logFile << "..." << endline;
  xpollog::kLogger->setLogFilePath(logFile);
  xpollog::kLogger->enableLogFile(true);
  // Save the run info.
  std::stringstream configurationInfo("");
  configurationInfo << *m_detectorConfiguration;
  std::stringstream preferencesInfo("");
  preferencesInfo << *m_userPreferences;
  *xpollog::kDebug << "Settings at start run...\n" << configurationInfo.str()
		   << preferencesInfo.str() << endline;
  saveRunInfo();
  resetRunInfo();
  m_timer->start();
  m_startSeconds = currentSeconds();
  *xpollog::kInfo << "Run controller started on " << startDatetime()
		  << " (" << m_startSeconds << " s since January 1, 1970)."
		  << endline;
  m_dataCollector->reset();
  if (m_usbController->IsOpened()) {
    m_usbController->setTimeout(m_userPreferences->usbTimeout());
    m_xpolFpga->setup(m_detectorConfiguration);
    m_dataCollector->setup(dataFilePath(), m_userPreferences,
			   m_detectorConfiguration);
    m_dataCollector->start();
  } else {
    *xpollog::kError << "The USB device is not open." << endline;
    exit(1);
  }
}

/*!
*/
void pRunController::fsmStopRun()
{
  *xpollog::kInfo << "Stopping run controller..." << endline;
  m_dataCollector->stop();
  m_stopSeconds = currentSeconds();
  m_timer->stop();
  *xpollog::kInfo << "Run controller stopped on " << stopDatetime()
		  << " (" << m_stopSeconds << " s since January 1, 1970)."
		  << endline;
  *xpollog::kInfo << numEvents() << " events (" <<
    numDataBlocks() << " data blocks) acquired in "<<
    elapsedSeconds() << " seconds."<< endline;
  *xpollog::kInfo << "Disconnecting logger from file..." << endline;
  xpollog::kLogger->enableLogFile(false);
}


/*!
 */
void pRunController::fsmPause()
{
  *xpollog::kInfo << "Run controller paused." << endline;
  m_dataCollector->stop();
}


/*!
 */
void pRunController::fsmResume()
{
  *xpollog::kInfo << "Run controller restarted." << endline;
  if (m_usbController->IsOpened()) {
    m_dataCollector->start();
  } else {
    *xpollog::kError << "The USB device is not open." << endline;
    exit(1);
  }
}


/*!
 */
void pRunController::fsmStop()
{
  fsmStopRun();
}


/*! Nothing to do here.
 */
void pRunController::fsmTeardown()
{

}


/*!
 */
std::string pRunController::outputFolderPath() const
{
  return xpedaqos::join(m_userPreferences->outputFolder(), baseFileName());
}


/*!
 */
std::string pRunController::baseFileName() const
{
  std::ostringstream name;
  name << std::setw(3) << std::setfill('0') << m_stationId << "_"
       << std::setw(7) << m_runId;
  return name.str();
}


/*!
 */
std::string pRunController::outputFilePath(std::string fileName) const
{
  return xpedaqos::join(outputFolderPath(), baseFileName() + "_" + fileName);
}


/*!
 */
std::string pRunController::dataFilePath() const
{
  return outputFilePath("data.mdat");
}


/*!
 */
std::string pRunController::logFilePath() const
{
  return outputFilePath("run.log");
}


/*!
 */
std::string pRunController::reportFilePath() const
{
  return outputFilePath("report.pdf");
}


/*!
 */
std::string pRunController::detectorConfigurationFilePath() const
{
  return outputFilePath("detector.cfg");
}


/*!
 */
std::string pRunController::userPreferencesFilePath() const
{
  return outputFilePath("preferences.cfg");
}


/*!
 */
std::string pRunController::xpedaqVersionFilePath() const
{
  return outputFilePath("version.h");
}


/*! This is actually saving a copy of the detector configuration both
  in the default locations (to be reused, e.g, when the GUI is restarted) and 
  in the output folder for permanent future reference.

  (This function is called at the start run, right before the data acquisition
  is actually started.)
*/
void pRunController::saveRunInfo() const
{
  m_detectorConfiguration->writeToFile(m_configFilePath);
  m_detectorConfiguration->writeToFile(detectorConfigurationFilePath());
  m_userPreferences->writeToFile(m_preferencesFilePath);
  m_userPreferences->writeToFile(userPreferencesFilePath());
  xpedaqos::copyFile(xpedaqos::rjoin("__version__.h"), xpedaqVersionFilePath());
}
