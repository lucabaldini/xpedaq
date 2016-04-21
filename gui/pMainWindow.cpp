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

#include "pMainWindow.h"

pMainWindow::pMainWindow()
{
  pUserPreferences preferences =
    pUserPreferences(xpolenv::kDefaultPreferencesFilePath);
  xpollog::kLogger->setTerminalLevel(preferences.getLoggerTerminalLevel());
  xpollog::kLogger->setDisplayLevel(preferences.getLoggerDisplayLevel());
  setupDaqDisplay();
  setupMessageDisplay();
  setupLoggerConnections();
  setupTransportBar();
  setupTabWidget();
  setupActions();
  setupMenuBar();
  m_runController = new pRunController(this);
  setupConnections();
  m_userPreferencesTab->displayUserPreferences(preferences);
  m_lastVisualizationMode = preferences.getVisualizationMode();
  selectConfiguration(xpolenv::kDefaultConfigFilePath);
  m_runController->init();
  showMessage("Data acquisition system ready", 2000);
}

pMainWindow::~pMainWindow()
{
  saveUserPreferences();
  saveConfiguration(false);
}

void pMainWindow::setupDaqDisplay()
{
  m_daqDisplay = new pDaqDisplay(m_centralWidget);
  m_daqDisplay->freezeSize(DISPLAYS_WIDTH, -1);
  m_mainGridLayout->addWidget(m_daqDisplay, 0, 0, Qt::AlignTop);
}

void pMainWindow::setupMessageDisplay()
{
  m_messageDisplay = new pMessageDisplay(m_centralWidget);
  m_messageDisplay->freezeSize(DISPLAYS_WIDTH, 200);
  m_mainGridLayout->addWidget(m_messageDisplay, 1, 0, Qt::AlignCenter);
}

void pMainWindow::setupTransportBar()
{
  m_transportBar   = new pTransportBar(m_centralWidget);
  m_mainGridLayout->addWidget(m_transportBar, 2, 0, Qt::AlignBottom);
}

void pMainWindow::setupTabWidget()
{
  m_mainTabWidget  = new pQtCustomTabWidget(m_centralWidget);
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

void pMainWindow::setupActions()
{
  m_selectConfigurationAction = new QAction("&Select configuration", this);
  m_selectConfigurationAction->setShortcut(tr("Ctrl+L"));
  m_selectConfigurationAction->setStatusTip("Select a configuration");
  connect(m_selectConfigurationAction, SIGNAL(triggered()),
	  this, SLOT(selectConfiguration()));
  m_saveConfigurationAction = new QAction("&Save current configuration", this);
  m_saveConfigurationAction->setShortcut(tr("Ctrl+S"));
  m_saveConfigurationAction->setStatusTip("Save the configuration");
  connect(m_saveConfigurationAction, SIGNAL(triggered()),
	  this, SLOT(saveConfiguration()));
  m_savePreferencesAction = new QAction("&Save preferences", this);
  m_savePreferencesAction->setShortcut(tr("Ctrl+P"));
  m_savePreferencesAction->setStatusTip("Save current user preferences");
  connect(m_savePreferencesAction, SIGNAL(triggered()),
	  this, SLOT(saveUserPreferences()));
}

void pMainWindow::setupMenuBar()
{
  m_fileMenu->addAction(m_selectConfigurationAction);
  m_fileMenu->addAction(m_saveConfigurationAction);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_savePreferencesAction);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_quitAction);
}

void pMainWindow::start()
{
  m_transportBar->pressStartButton();
}

void pMainWindow::stop()
{
  m_transportBar->pressStopButton();
}

void pMainWindow::disableHardware()
{
  m_transportBar->setEnabled(0);
  m_thresholdSettingTab->getRefreshRefButton()->setEnabled(0);
}

pDetectorConfiguration *pMainWindow::getConfiguration(int mode)
{
  if (mode == -1){
    mode = getVisualizationMode();
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

pUserPreferences *pMainWindow::getUserPreferences()
{
  return m_userPreferencesTab->getUserPreferences();
}

int pMainWindow::getVisualizationMode()
{
  return getUserPreferences()->getVisualizationMode();
}

void pMainWindow::selectConfiguration()
{
  QString windowTitle = "Select a configuration";
  QString defaultDir  = QString::fromStdString(xpolenv::kDaqConfigDirPath);
  QString filePattern = "xPol configuration files (*.cfg)";
  QString filePath    = QFileDialog::getOpenFileName(this, windowTitle,
						     defaultDir, filePattern);
  if (!(filePath.isNull())){
    selectConfiguration((char*)filePath.toLocal8Bit().constData());
  }
}

void pMainWindow::selectConfiguration(std::string filePath)
{
  pDetectorConfiguration configuration = pDetectorConfiguration();
  configuration.readFromFile(filePath);
  displayConfiguration(&configuration,
		       getUserPreferences()->getVisualizationMode());
}

void pMainWindow::saveConfiguration(bool promptDialog)
{
  if (promptDialog)
    {
      QString windowTitle = "Save the current configuration";
      QString defaultDir = QString::fromStdString(xpolenv::kDaqConfigDirPath);
      QString filePattern = "xPol configuration files (*.cfg)";
      QString filePath = QFileDialog::getSaveFileName(this, windowTitle,
						      defaultDir, filePattern);
      if (!(filePath.isNull())){
	getConfiguration()->writeToFile(filePath.toStdString());
      }
    } else {
    getConfiguration()->writeToFile(xpolenv::kDefaultConfigFilePath);
  }
}

void pMainWindow::saveUserPreferences()
{
  getUserPreferences()->writeToFile(xpolenv::kDefaultPreferencesFilePath);
}

void pMainWindow::displayConfiguration(pDetectorConfiguration *configuration,
				       int mode)
{
  m_readoutModeTab->displayConfiguration(configuration, mode);
  m_thresholdSettingTab->displayConfiguration(configuration, mode);
  m_advancedSettingsTab->displayConfiguration(configuration);
}

void pMainWindow::displayReference(unsigned short reference)
{
  m_thresholdSettingTab->displayReference(reference, getVisualizationMode());
}

void pMainWindow::changeVisualizationMode(int mode)
{
  displayConfiguration(getConfiguration(m_lastVisualizationMode), mode);
  m_lastVisualizationMode = mode;
}

void pMainWindow::setupLoggerConnections()
{
  connect(xpollog::kDebug, SIGNAL(message(QString)),
	  m_messageDisplay, SLOT(showMessage(QString)));
  connect(xpollog::kInfo, SIGNAL(message(QString)),
	  m_messageDisplay, SLOT(showMessage(QString)));
  connect(xpollog::kWarning, SIGNAL(message(QString)),
	  m_messageDisplay, SLOT(showMessage(QString)));
  connect(xpollog::kError, SIGNAL(message(QString)),
	  m_messageDisplay, SLOT(showMessage(QString)));
}

void pMainWindow::setupConnections()
{
  connect(m_transportBar, SIGNAL(start()),
	  m_runController, SLOT(start()));
  connect(m_transportBar, SIGNAL(stop()),
	  m_runController, SLOT(stop()));
  connect(m_transportBar, SIGNAL(pause()),
	  m_runController, SLOT(pause()));
  connect(m_transportBar, SIGNAL(reset()),
	  m_runController, SLOT(reset()));
  connect(m_runController, SIGNAL(numAcquiredDataBlocksChanged(int)),
	  m_daqDisplay, SLOT(updateNumDataBlocks(int)));
  connect(m_runController, SIGNAL(numAcquiredEventsChanged(int)),
	  m_daqDisplay, SLOT(updateNumEvents(int)));
  connect(m_runController, SIGNAL(runIdChanged(int)),
	  m_daqDisplay, SLOT(updateRunId(int)));
  connect(m_runController, SIGNAL(statusChanged(QString)),
	  m_daqDisplay, SLOT(updateStatus(QString)));
  connect(m_runController, SIGNAL(elapsedSecondsChanged(int)),
	  m_daqDisplay, SLOT(updateElapsedSeconds(int)));
  connect(m_runController, SIGNAL(averageEventRateChanged(double)),
	  m_daqDisplay, SLOT(updateAverageDaqRate(double)));
  connect(m_runController, SIGNAL(instantEventRateChanged(double)),
	  m_daqDisplay, SLOT(updateInstantDaqRate(double)));
  connect(m_runController, SIGNAL(instantFpgaEventRateChanged(double)),
	  m_daqDisplay, SLOT(updateInstantFpgaRate(double)));
  connect(m_runController->getXpolFpga(),
	  SIGNAL(thresholdRefRead(unsigned short)),
	  this, SLOT(displayReference(unsigned short)));
  connect(m_thresholdSettingTab->getRefreshRefButton(), SIGNAL(clicked()),
  	  m_runController->getXpolFpga(), SLOT(readVrefDac()));
  connect(m_userPreferencesTab, SIGNAL(visualizetionModeChanged(int)),
	  this, SLOT(changeVisualizationMode(int))); 
}
