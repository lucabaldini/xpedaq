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


#include "xpedaqWindow.h"


xpedaqWindow::xpedaqWindow(pRunController &runController) :
  pAcquisitionWindow(runController)
{
  QString title = "xpedaq version " + QString(__XPEDAQ_VERSION__);
  setWindowTitle(title);
  setupTabWidget();
  pTriggerMask *triggerMask = m_runController->triggerMask();
  displayTriggerMask(triggerMask);
  pUserPreferences *preferences = m_runController->userPreferences();
  displayUserPreferences(preferences);
  m_lastVisualizationMode = preferences->visualizationMode();
  pDetectorConfiguration *configuration =
    m_runController->detectorConfiguration();  
  displayConfiguration(configuration, preferences->visualizationMode());
  displayUserComment(m_runController->userComment());
  setupConnections();
  m_runController->init();
  showMessage("Data acquisition system ready", 2000);

}

/*!
 */
void xpedaqWindow::setupConnections()
{
}


void xpedaqWindow::setupTabWidget()
{
  m_readoutModeTab = new pReadoutModeTab();
  m_mainTabWidget->insertTab(0, m_readoutModeTab, "Readout");
  m_triggerSettingTab = new pTriggerSettingTab();
  m_mainTabWidget->addTab(m_triggerSettingTab, "Trigger");
  m_mainTabWidget->setCurrentWidget(m_readoutModeTab);
}


/*!
 */
void xpedaqWindow::displayConfiguration(pDetectorConfiguration *configuration,
				                        int mode)
{
  m_readoutModeTab->displayConfiguration(configuration, mode);
  m_thresholdSettingTab->displayConfiguration(configuration, mode);
  m_advancedSettingsTab->displayConfiguration(configuration);
}


/*!
 */
pTriggerMask* xpedaqWindow::triggerMask()
{
  return m_triggerSettingTab->triggerMask();
}


/*!
 */
pDetectorConfiguration* xpedaqWindow::detectorConfiguration(int mode)
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
  configuration->setClockFrequency(m_advancedSettingsTab->clockFrequency());
  configuration->setClockShift(m_advancedSettingsTab->clockShift());
  configuration->setNumPedSamples(m_advancedSettingsTab->numPedSamples());
  configuration->setPedSampleDelay(m_advancedSettingsTab->pedSubDelay());
  configuration->setTrgEnableDelay(m_advancedSettingsTab->trgEnableDelay());
  configuration->setMinWindowSize(m_advancedSettingsTab->minWindowSize());
  configuration->setMaxWindowSize(m_advancedSettingsTab->maxWindowSize());
  configuration->setWindowMarginHigh(m_advancedSettingsTab->windowMarginHigh());
  return configuration;
}


/*!
 */
void xpedaqWindow::enableTabs()
{
  pAcquisitionWindow::enableTabs();
  m_readoutModeTab->setEnabled(1);
  m_triggerSettingTab->setEnabled(1);
}


/*!
 */
void xpedaqWindow::disableTabs()
{
  pAcquisitionWindow::disableTabs();
  m_readoutModeTab->setEnabled(0);
  m_triggerSettingTab->setEnabled(0);
}
