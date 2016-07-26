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

#include "pAcquisitionWindow.h"


pAcquisitionWindow::pAcquisitionWindow(pRunController &runController)
{  
  setupDaqDisplay();
  setupMessageDisplay();
  setupLoggerConnections();
  setupTransportBar();
  setupTabWidget();
  setupCommentBox();  
  m_runController = &runController;
  // This connection needs to be here in order to intercept error signals.
  connect(m_runController->usbController(),
    SIGNAL(quickusbError(unsigned long)),
    this, SLOT(disableHardwareWidgets()));
  connect(m_runController->usbController(),
	  SIGNAL(connected(QString, QString, QString, QString)),
	  m_usbControlTab,
	  SLOT(updateInfo(QString, QString, QString, QString)));
  m_runController->connectUsb();
  setupConnections();
}

pAcquisitionWindow::~pAcquisitionWindow()
{
  // Do we want to save the detector config and user preferences, here?
  // (Note that this is done at the start run.)
  delete m_runController;
}

/*! Pass the configuration from the GUI to the run controller and start the run.
 */
void pAcquisitionWindow::startRun()
{
  m_runController->setupRun(detectorConfiguration(), userPreferences(),
                            triggerMask(), m_commentBox->comment());
  m_runController->setRunning();
}

/*! Stop the run controller.
 */
void pAcquisitionWindow::stopRun()
{
  m_runController->setStopped();
}


/*!
 */
void pAcquisitionWindow::setupDaqDisplay()
{
  m_daqDisplay = new pDaqDisplay(m_centralWidget);
  m_daqDisplay->freezeSize(DISPLAYS_WIDTH, -1);
  m_mainGridLayout->addWidget(m_daqDisplay, 0, 0, Qt::AlignTop);
}


/*!
 */
void pAcquisitionWindow::setupMessageDisplay()
{
  m_messageDisplay = new pMessageDisplay(m_centralWidget);
  m_messageDisplay->freezeSize(DISPLAYS_WIDTH, 150);
  m_mainGridLayout->addWidget(m_messageDisplay, 1, 0, Qt::AlignCenter);
}

/*!
 */
void pAcquisitionWindow::setupCommentBox()
{
  m_commentBox = new pCommentBoxWidget(m_centralWidget);
  m_mainGridLayout->addWidget(m_commentBox, 2, 0, Qt::AlignBottom);
}

/*!
 */
void pAcquisitionWindow::setupTransportBar()
{
  m_transportBar   = new pTransportBar(m_centralWidget);
  m_mainGridLayout->addWidget(m_transportBar, 3, 0, Qt::AlignBottom);
}


/*!
 */
void pAcquisitionWindow::setupTabWidget()
{
  m_mainTabWidget = new QTabWidget(m_centralWidget);
  m_mainGridLayout->addWidget(m_mainTabWidget, 0, 1, 3, 1);
  m_thresholdSettingTab = new pThresholdSettingTab();
  m_mainTabWidget->addTab(m_thresholdSettingTab, "Thresholds");
  m_advancedSettingsTab = new pAdvancedSettingsTab();
  m_mainTabWidget->addTab(m_advancedSettingsTab, "Advanced");
  m_usbControlTab = new pUsbControlTab();
  m_mainTabWidget->addTab(m_usbControlTab, "USB");
  m_userPreferencesTab = new pUserPreferencesTab();
  m_mainTabWidget->addTab(m_userPreferencesTab, "Preferences");
}

/*!
 */
void pAcquisitionWindow::start()
{
  m_transportBar->pressStartButton();
}

/*!
 */
void pAcquisitionWindow::stop()
{
  m_transportBar->pressStopButton();
}

/*!
 */
void pAcquisitionWindow::disableHardwareWidgets()
{
  m_transportBar->setEnabled(0);
  m_thresholdSettingTab->getRefreshRefButton()->setEnabled(0);
}

/*! Disable all the tabs
*/
void pAcquisitionWindow::disableTabs()
{
  m_thresholdSettingTab->getRefreshRefButton()-> setEnabled(0);
  m_thresholdSettingTab->setEnabled(0);
  m_advancedSettingsTab->setEnabled(0);
  m_usbControlTab->setEnabled(0);
  m_userPreferencesTab->setEnabled(0);
}

/*!
*/
void pAcquisitionWindow::enableTabs()
{
  m_thresholdSettingTab->getRefreshRefButton()-> setEnabled(1);
  m_thresholdSettingTab->setEnabled(1);
  m_advancedSettingsTab->setEnabled(1);
  m_usbControlTab->setEnabled(1);
  m_userPreferencesTab->setEnabled(1);
}

/*!
 */
pUserPreferences *pAcquisitionWindow::userPreferences()
{
  pUserPreferences *preferences = 
                          m_userPreferencesTab->getUserPreferences();
  preferences->setUsbTimeout(m_usbControlTab->timeout());
  return preferences;
}

/*!
 */
pTriggerMask *pAcquisitionWindow::triggerMask()
{
  pTriggerMask* mask = new pTriggerMask();
  return mask;
}

/*!
 */
int pAcquisitionWindow::visualizationMode()
{
  return userPreferences()->visualizationMode();
}

/*!
 */
void pAcquisitionWindow::displayConfiguration(pDetectorConfiguration *configuration,
				       int mode)
{
  m_thresholdSettingTab->displayConfiguration(configuration, mode);
  m_advancedSettingsTab->displayConfiguration(configuration);
}

/*!
 */
void pAcquisitionWindow::displayUserPreferences(pUserPreferences *preferences)
{
  m_userPreferencesTab->displayUserPreferences(*preferences);
  m_usbControlTab->setTimeout(preferences->usbTimeout());
}

/*!
 */
void pAcquisitionWindow::displayTriggerMask(pTriggerMask *triggerMask)
{
  m_triggerSettingTab->displayTriggerMask(triggerMask);
}

/*!
 */
void pAcquisitionWindow::displayReference(unsigned short reference)
{
  m_thresholdSettingTab->displayReference(reference, visualizationMode());
}

/*!
 */
void pAcquisitionWindow::displayUserComment(std::string userComment){
  m_commentBox->setText(userComment);
}
  
/*!
 */
void pAcquisitionWindow::changeVisualizationMode(int mode)
{
  displayConfiguration(detectorConfiguration(m_lastVisualizationMode), mode);
  m_lastVisualizationMode = mode;
}

/*!
 */
void pAcquisitionWindow::setupLoggerConnections()
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
void pAcquisitionWindow::setupConnections()
{
  connect(m_transportBar, SIGNAL(start()), this, SLOT(startRun()));    
  connect(m_transportBar, SIGNAL(stop()), this, SLOT(stopRun()));

  connect(m_runController, SIGNAL(runStarted()),
          this, SLOT(disableTabs()));
  connect(m_runController, SIGNAL(runStopped()),
          this, SLOT(enableTabs()));          
  connect(m_runController, SIGNAL(runStopped()), this, SLOT(stop()));
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
  connect(m_runController->xpolFpga(),
	  SIGNAL(thresholdRefRead(unsigned short)),
	  this, SLOT(displayReference(unsigned short)));
  connect(m_thresholdSettingTab->getRefreshRefButton(), SIGNAL(clicked()),
  	  m_runController->xpolFpga(), SLOT(readVrefDac()));
  connect(m_userPreferencesTab, SIGNAL(visualizetionModeChanged(int)),
	  this, SLOT(changeVisualizationMode(int)));
}
