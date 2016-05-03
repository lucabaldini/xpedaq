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

#include "pFiniteStateMachine.h"
#include "pDataCollector.h"
#include "pUsbController.h"
#include "pXpolFpga.h"
#include "xpedaqos.h"
#include "xpollog.h"
#include "xpolio.h"


class pRunController : public pFiniteStateMachine
{
  
  Q_OBJECT
  
 public:
  
  pRunController(int maxSeconds = 604800, int maxEvents = 100000000,
		 int maxDataBlocks = 50000000);
  ~pRunController() {;}
  unsigned long connectUsb();
  void init();
  inline pDataCollector *dataCollector() const {return m_dataCollector;}
  inline pUsbController *usbController() const {return m_usbController;}
  inline pXpolFpga *xpolFpga() const {return m_xpolFpga;}
  inline pDetectorConfiguration *detectorConfiguration()
    const {return m_detectorConfiguration;}
  inline pUserPreferences *userPreferences() const {return m_userPreferences;}

  /// \brief Setup all the relevant run information.
  void setupRun(pDetectorConfiguration *configuration,
		pUserPreferences *preferences);

  /// \brief Same thing but reading the info from file.
  void setupRun(std::string configFilePath, std::string preferencesFilePath);

  /// \brief Same thing but reading the stuff from the default files.
  void setupRun();
  
  /// \brief Return the current value of the system time.
  int currentSeconds() const;

  /// \brief Return the time elapsed since the start run. 
  int elapsedSeconds() const;

  int numDataBlocks() const {return m_dataCollector->numDataBlocks();}
  int numEvents() const {return m_dataCollector->numEvents();}
  double averageEventRate() const;
  double instantEventRate() const;
  void setMaxSeconds(int maxSeconds) {m_maxSeconds = maxSeconds;}
  void setMaxDataBlocks(int maxDataBlocks) {m_maxDataBlocks = maxDataBlocks;}
  void setMaxEvents(int maxEvents) {m_maxEvents = maxEvents;}

  
 public slots:

  
  
 signals:

  void stationIdSet(int stationId);
  void runIdChanged(int runId);
  void elapsedSecondsChanged(int elapsedSeconds);
  void numDataBlocksChanged(int numDataBlocks);
  void numEventsChanged(int numEvents);
  void averageEventRateChanged(double rate);
  void instantEventRateChanged(double rate);

  
 protected:
  
  void fsmSetup();
  void fsmTeardown();
  void fsmStartRun();
  void fsmStopRun();
  void fsmPause();
  void fsmResume();
  void fsmStop();

  
 private:

  /// \brief Maximum duration (in s) for the data acquisition.
  int m_maxSeconds;

  /// \brief Maximum number of events to be collected.
  int m_maxEvents;

  /// \brief Maximum number of data blocks to be collected.
  int m_maxDataBlocks;

  /// \brief Path to the configuration file containing the station identifier.
  std::string m_stationIdFilePath;

  /// \brief Station identifier for the machine where the DAQ is running.
  int m_stationId;

  /// \brief Read the station identifier from the proper file.
  int readStationId() const;

  /// \brief Set the current station identifier.
  void setStationId(int stationId);

  /// \brief Path to the configuration file containing the run identifier.
  std::string m_runIdFilePath;

  /// \brief Current run identifier.
  int m_runId;

  /// \brief Read the current run identifier from the proper file.
  int readRunId() const;

  /// \brief Write the current run identifier to the proper file.
  void writeRunId() const;

  /// \brief Set the current run identifier.
  void setRunId(int runId);

  /// \brief Increment the current run identifier.
  void incrementRunId();

  /// \brief QTimer object for the data acquisition.
  QTimer *m_timer;

  /// \brief The value of the system time, latched at the start run.
  int m_startSeconds;

  /// \brief The value of the system time, latched at the stop run.
  int m_stopSeconds;

  /// \brief Path to the default detector configuration file path.
  std::string m_configFilePath;

  /// \brief Path to the default user preferences configuration file path.
  std::string m_preferencesFilePath;
  
  /// \brief Return the path to the current output folder.
  std::string outputFolderPath() const;

  /// \brief Return the base string to compose the paths to the output files.
  std::string baseFileName() const;

  /// \brief Return the path to the current output data file. 
  std::string dataFilePath() const;

  /// \brief Return the path to the current log file.
  std::string logFilePath() const;

  /// \brief Return the path to the curret run report.
  std::string reportFilePath() const;

  /// \brief Return the path to the (output copy) of the configuration file.
  std::string detectorConfigurationFilePath() const;

  /// \brief Return the path to the (output copy) of the preferences file.
  std::string userPreferencesFilePath() const;

  /// \brief Save the run info into the output folder.
  void saveRunInfo() const;

  /// \brief Pointer to the USB controller member object.
  pUsbController *m_usbController;

  /// \brief Pointer to the data collector member object.
  pDataCollector *m_dataCollector;

  /// \brief Pointer to the FPGA member object.
  pXpolFpga *m_xpolFpga;

  /// \brief Pointer to the detector configuration member object.
  pDetectorConfiguration *m_detectorConfiguration;

  /// \brief Pointer to the user preferences member object.
  pUserPreferences *m_userPreferences;
  

 private slots:
   
  void updateRunInfo();
  void resetRunInfo();
};

#endif //PRUNCONTROLLER_H
