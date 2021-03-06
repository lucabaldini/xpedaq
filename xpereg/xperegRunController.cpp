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


#include "xperegRunController.h"


xperegRunController::xperegRunController(std::string preferencesFilePath) :
  m_preferencesFilePath(preferencesFilePath)
{
  m_stationIdFilePath = xpedaqos::rjoin("config", "stationId.cfg");
  if (!xpedaqos::fileExists(m_stationIdFilePath)) {
    *xpollog::kError << "Could not find " << m_stationIdFilePath << endline;
    exit(1);
  }
  m_runIdFilePath = xpedaqos::rjoin("config", "runId.cfg");
  if (!xpedaqos::fileExists(m_preferencesFilePath)) {
    xpedaqos::copyFile(m_preferencesFilePath + ".sample",
		       m_preferencesFilePath);
  }
  setupRun();
  m_timer = new QTimer();
  m_timer->setInterval(1000);
  m_chrono = new pChrono();
  connect(m_timer, SIGNAL(timeout()), this, SLOT(updateRunInfo()));
  m_usbController = new pUsbController();
  m_xpolFpga = new pXpolFpga(m_usbController);
  m_registerPoker = new pXpolRegisterPoker(m_xpolFpga);
  connect(this, SIGNAL(runStarted()), m_registerPoker, SLOT(start()));
  connect(this, SIGNAL(runStopped()), m_registerPoker, SLOT(stop()));
  // This is not used and we should get rid of it?
  m_dataCollector = new pDataCollector(m_xpolFpga, false);
}                                   


void xperegRunController::setupRun(xperegUserPreferences *preferences)
{
  *xpollog::kInfo << "Setting up run..." << endline;
  m_userPreferences = preferences;
  // The following two lines are duplicated and should be refactored.
  xpollog::kLogger->setTerminalLevel(m_userPreferences->m_loggerTerminalLevel);
  xpollog::kLogger->setDisplayLevel(m_userPreferences->m_loggerDisplayLevel);
}


void xperegRunController::setupRun(std::string preferencesFilePath)
{
  m_userPreferences = new xperegUserPreferences(preferencesFilePath);
  xpollog::kLogger->setTerminalLevel(m_userPreferences->m_loggerTerminalLevel);
  xpollog::kLogger->setDisplayLevel(m_userPreferences->m_loggerDisplayLevel);
}


/*!
 */
void xperegRunController::setupRun()
{
  setupRun(m_preferencesFilePath);
}


/*! Nothing to do here.
 */
void xperegRunController::fsmSetup()
{

}


/*!
 */
void xperegRunController::fsmStartRun()
{
  *xpollog::kInfo << "Starting run controller..." << endline;
  incrementRunId();
  // Create the output folder.
  std::string outputFolder = outputFolderPath();
  if (!xpedaqos::folderExists(outputFolder)) {
    xpedaqos::mkdir(outputFolder);
  }
  // Set the log file path.
  std::string logFile = logFilePath();
  *xpollog::kInfo << "Redirecting logger to " << logFile << "..." << endline;
  xpollog::kLogger->setLogFilePath(logFile);
  xpollog::kLogger->enableLogFile(true);
  // Print out the test configuration.
  std::stringstream preferencesInfo("");
  preferencesInfo << *m_userPreferences;
  *xpollog::kDebug << "Settings at start run...\n" << preferencesInfo.str()
		   << endline;  
  saveRunInfo();
  resetRunInfo();
  m_timer->start();
  m_startSeconds = currentSeconds();
  m_chrono->start();
  *xpollog::kInfo << "Run controller started on " << m_chrono->startDateTime()
		  << endline;
  m_registerPoker->setup(m_userPreferences);
  m_registerPoker->reset();
  if (m_usbController->IsOpened()) {
    m_usbController->setTimeout(1000);
    m_registerPoker->moveToThread(&m_thread);
    m_thread.start();
  } else {
    *xpollog::kError << "The USB device is not open." << endline;
    exit(1);
  } 
  emit runStarted();
}


/*!
*/
void xperegRunController::fsmStopRun()
{
  *xpollog::kInfo << "Stopping run controller..." << endline;
  m_stopSeconds = currentSeconds();
  m_timer->stop();
  m_chrono->stop();
  *xpollog::kInfo << "Run controller stopped on " << m_chrono->stopDateTime()
		  << endline;
  *xpollog::kInfo << m_registerPoker->numPokes() << " poke(s) ("
		  << m_registerPoker->numReadouts() << " readouts, "
		  << m_registerPoker->numReadoutErrors()
		  << " readout errors) performed in "
		  << m_chrono->elapsedSeconds() << " s." << endline;
  *xpollog::kInfo << "Disconnecting logger from file..." << endline;
  xpollog::kLogger->enableLogFile(false);
  writeRunStat(runStatFilePath());
  emit runStopped();
  m_thread.quit();
  m_thread.wait();
}


/*!
 */
void xperegRunController::fsmPause()
{
  
}


/*!
 */
void xperegRunController::fsmResume()
{

}


/*!
 */
void xperegRunController::fsmStop()
{
  fsmStopRun();
}


/*! Nothing to do here.
 */
void xperegRunController::fsmTeardown()
{

}


/*!
 */
std::string xperegRunController::outputFolderPath() const
{
  return xpedaqos::join(m_userPreferences->m_outputFolder, baseFileName());
}


void xperegRunController::saveRunInfo() const
{
  m_userPreferences->writeToFile(m_preferencesFilePath);
  m_userPreferences->writeToFile(userPreferencesFilePath());
  xpedaqos::copyFile(xpedaqos::rjoin("__version__.h"), xpedaqVersionFilePath());
}


void xperegRunController::updateRunInfo()
{
  emit elapsedSecondsChanged(elapsedSeconds());
  emit numPokesChanged(m_registerPoker->numPokes());
  emit numReadoutsChanged(m_registerPoker->numReadouts());
  emit numReadoutErrorsChanged(m_registerPoker->numReadoutErrors());
}


void xperegRunController::resetRunInfo()
{
  emit elapsedSecondsChanged(0);
  emit numPokesChanged(0);
  emit numReadoutsChanged(0);
  emit numReadoutErrorsChanged(0);
}


void xperegRunController::writeRunStat(std::string filePath) const
{
  *xpollog::kInfo << "Writing run statistics to " << filePath <<
    "... " << endline;
  std::ofstream *outputFile = xpolio::kIOManager->openOutputFile(filePath);
  *outputFile << "Start date/time: " << startDatetime() << std::endl;
  *outputFile << "Start seconds: " << m_startSeconds << std::endl;
  *outputFile << "Stop date/time: " << stopDatetime() << std::endl;
  *outputFile << "Stop seconds: " << m_stopSeconds << std::endl;
  *outputFile << "Run duration [s]: " << m_chrono->elapsedSeconds()
	      << std::endl;
  *outputFile << "Number of pokes: " << m_registerPoker->numPokes()
	      << std::endl;
  *outputFile << "Number of readouts: " << m_registerPoker->numReadouts()
	      << std::endl;
  *outputFile << "Number of readout errors: "
	      << m_registerPoker->numReadoutErrors() << std::endl;  
  xpolio::kIOManager->closeOutputFile(outputFile);
}
