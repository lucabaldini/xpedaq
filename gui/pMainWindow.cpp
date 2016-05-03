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

#include "pMainWindow.h"


pMainWindow::pMainWindow()
{  
  setupDaqDisplay();
  setupMessageDisplay();
  setupLoggerConnections();
  setupTransportBar();
  setupTabWidget();
  m_runController = new pRunController();
  // This connection needs to be here in order to intercept error signals
  // from QuickUsb.
  connect(m_runController, SIGNAL(quickusbError(unsigned long)),
	  this, SLOT(disableHardwareWidgets()));
  m_runController->connectToQuickUsb();
  setupConnections();
  pUserPreferences *preferences = m_runController->userPreferences();
  m_userPreferencesTab->displayUserPreferences(*preferences);
  m_lastVisualizationMode = preferences->visualizationMode();
  pDetectorConfiguration *configuration =
    m_runController->detectorConfiguration();  
  displayConfiguration(configuration, preferences->visualizationMode());
  m_runController->init();
  showMessage("Data acquisition system ready", 2000);
}

pMainWindow::~pMainWindow()
{
  // Do we want to save the detector config and user preferences, here?
  // (Note that this is done at the start run.)
  delete m_runController;
}


/*! Pass the configuration from the GUI to the run controller and start the run.
 */
void pMainWindow::startRun()
{
  m_runController->setupRun(detectorConfiguration(), userPreferences());
  m_runController->setRunning();
}


/*! Stop the run controller.
 */
void pMainWindow::stopRun()
{
  m_runController->setStopped();
}


/*!
 */
void pMainWindow::setupDaqDisplay()
{
  m_daqDisplay = new pDaqDisplay(m_centralWidget);
  m_daqDisplay->freezeSize(DISPLAYS_WIDTH, -1);
  m_mainGridLayout->addWidget(m_daqDisplay, 0, 0, Qt::AlignTop);
}


/*!
 */
void pMainWindow::setupMessageDisplay()
{
  m_messageDisplay = new pMessageDisplay(m_centralWidget);
  m_messageDisplay->freezeSize(DISPLAYS_WIDTH, 200);
  m_mainGridLayout->addWidget(m_messageDisplay, 1, 0, Qt::AlignCenter);
}


/*!
 */
void pMainWindow::setupTransportBar()
{
  m_transportBar   = new pTransportBar(m_centralWidget);
  m_mainGridLayout->addWidget(m_transportBar, 2, 0, Qt::AlignBottom);
}


/*!
 */
void pMainWindow::setupTabWidget()
{
  m_mainTabWidget = new QTabWidget(m_centralWidget);
  m_mainGridLayout->addWidget(m_mainTabWidget, 0, 1, 3, 1);
  m_readoutModeTab = new pReadoutModeTab();
  m_mainTabWidget->addTab(m_readoutModeTab, "Readout");
  m_thresholdSettingTab = new pThresholdSettingTab();
  m_mainTabWidget->addTab(m_thresholdSettingTab, "Thresholds");
  m_advancedSettingsTab = new pAdvancedSettingsTab();
  m_mainTabWidget->addTab(m_advancedSettingsTab, "Advanced");
  m_triggerSettingTab = new pTriggerSettingTab();
  m_mainTabWidget->addTab(m_triggerSettingTab, "Trigger");
  m_probesSettingTab = new pProbeSettingTab();
  m_mainTabWidget->addTab(m_probesSettingTab, "Probes");
  m_usbControlTab = new pUsbControlTab();
  m_mainTabWidget->addTab(m_usbControlTab, "USB");
  m_userPreferencesTab = new pUserPreferencesTab();
  m_mainTabWidget->addTab(m_userPreferencesTab, "Preferences");
}


/*!
 */
void pMainWindow::start()
{
  m_transportBar->pressStartButton();
}


/*!
 */
void pMainWindow::stop()
{
  m_transportBar->pressStopButton();
}


/*!
 */
void pMainWindow::disableHardwareWidgets()
{
  m_transportBar->setEnabled(0);
  m_thresholdSettingTab->getRefreshRefButton()->setEnabled(0);
}


/*!
 */
pDetectorConfiguration *pMainWindow::detectorConfiguration(int mode)
{
  if (mode == -1){
    mode = visualizationMode();
  }
  pDetectorConfiguration *configuration = new pDetectorConfiguration();
  configuration->setReadoutMode(m_readoutModeTab->getReadoutMode());
  configuration->setBufferMode(m_readoutModeTab->getBufferMode());
  configuration->setCalibrationDac(m_readoutModeTab->
				   getCalibrationSignal(mode));
  configuration->setPixelAddressX(m_readoutModeTab->getPixelAddressX());
  configuration->setPixelAddressY(m_readoutModeTab->getPixelAddressY());
  for (int i = 0; i < NUM_READOUT_CLUSTERS; i++)
    {
      configuration->
	setThresholdDac(i, m_thresholdSettingTab->getThreshold(i, mode));
    }
  configuration->setTimingCode(m_advancedSettingsTab->getTimingCode());
  configuration->setNumPedSamples(m_advancedSettingsTab->getNumPedSamples());
  configuration->setPedSampleDelay(m_advancedSettingsTab->getPedSubDelay());
  configuration->setTrgEnableDelay(m_advancedSettingsTab->getTrgEnableDelay());
  configuration->setMinWindowSize(m_advancedSettingsTab->getMinWindowSize());
  configuration->setMaxWindowSize(m_advancedSettingsTab->getMaxWindowSize());
  return configuration;
}


/*!
 */
pUserPreferences *pMainWindow::userPreferences()
{
  return m_userPreferencesTab->getUserPreferences();
}


/*!
 */
int pMainWindow::visualizationMode()
{
  return userPreferences()->visualizationMode();
}


/*!
 */
void pMainWindow::displayConfiguration(pDetectorConfiguration *configuration,
				       int mode)
{
  m_readoutModeTab->displayConfiguration(configuration, mode);
  m_thresholdSettingTab->displayConfiguration(configuration, mode);
  m_advancedSettingsTab->displayConfiguration(configuration);
}


/*!
 */
void pMainWindow::displayReference(unsigned short reference)
{
  m_thresholdSettingTab->displayReference(reference, visualizationMode());
}


/*!
 */
void pMainWindow::changeVisualizationMode(int mode)
{
  displayConfiguration(detectorConfiguration(m_lastVisualizationMode), mode);
  m_lastVisualizationMode = mode;
}


/*!
 */
void pMainWindow::setupLoggerConnections()
{
  connect(xpollog::kDebug, SIGNAL(message(QString)), m_messageDisplay,
	  SLOT(showMessage(QString)));
  connect(xpollog::kInfo, SIGNAL(message(QString)), m_messageDisplay,
	  SLOT(showMessage(QString)));
  connect(xpollog::kWarning, SIGNAL(message(QString)), m_messageDisplay,
	  SLOT(showMessage(QString)));
  connect(xpollog::kError, SIGNAL(message(QString)), m_messageDisplay,
	  SLOT(showMessage(QString)));
}


/*!
 */
void pMainWindow::setupConnections()
{
  connect(m_transportBar, SIGNAL(start()), this, SLOT(startRun()));
  connect(m_transportBar, SIGNAL(stop()), this, SLOT(stopRun()));

  connect(m_runController, SIGNAL(stationIdSet(int)), m_daqDisplay,
	  SLOT(updateStationId(int)));
  connect(m_runController, SIGNAL(runIdChanged(int)), m_daqDisplay,
	  SLOT(updateRunId(int)));
  connect(m_runController, SIGNAL(statusChanged(QString)), m_daqDisplay,
	  SLOT(updateStatus(QString)));
  connect(m_runController, SIGNAL(numDataBlocksChanged(int)),
	  m_daqDisplay, SLOT(updateNumDataBlocks(int)));
  connect(m_runController, SIGNAL(numEventsChanged(int)),
	  m_daqDisplay, SLOT(updateNumEvents(int)));
  connect(m_runController, SIGNAL(elapsedSecondsChanged(int)),
	  m_daqDisplay, SLOT(updateElapsedSeconds(int)));
  connect(m_runController, SIGNAL(averageEventRateChanged(double)),
	  m_daqDisplay, SLOT(updateAverageDaqRate(double)));
  connect(m_runController, SIGNAL(instantEventRateChanged(double)),
	  m_daqDisplay, SLOT(updateInstantDaqRate(double)));
  connect(m_runController->getXpolFpga(),
	  SIGNAL(thresholdRefRead(unsigned short)),
	  this, SLOT(displayReference(unsigned short)));
  connect(m_thresholdSettingTab->getRefreshRefButton(), SIGNAL(clicked()),
  	  m_runController->getXpolFpga(), SLOT(readVrefDac()));
  connect(m_userPreferencesTab, SIGNAL(visualizetionModeChanged(int)),
	  this, SLOT(changeVisualizationMode(int)));
}
