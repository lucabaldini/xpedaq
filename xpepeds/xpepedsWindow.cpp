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


#include "xpepedsWindow.h"
#include <iostream>


xpepedsWindow::xpepedsWindow(pedRunController &runController) :
  pAcquisitionWindow(runController), m_pedRunController(&runController)
{
  QString title = "xpepeds version " + QString(__XPEDAQ_VERSION__);
  setWindowTitle(title);
  disableUnusedWidgets();
  m_isWindowOpen = false;
  setupConnections();
  pUserPreferences *preferences = m_runController->userPreferences();
  displayUserPreferences(preferences);
  m_lastVisualizationMode = preferences->visualizationMode();
  pDetectorConfiguration *configuration =
    m_runController->detectorConfiguration();  
  displayConfiguration(configuration, preferences->visualizationMode());  
  displayUserComment(m_runController->userComment());
  m_runController->init();
  showMessage("Data acquisition system ready", 2000);
}


/*
*/
void xpepedsWindow::disableUnusedWidgets()
{
  m_advancedSettingsTab->disableNumPedSamplesComboBox();
  m_advancedSettingsTab->disableMaxWindowSizeSpinBox();
  m_advancedSettingsTab->disableMinWindowSizeSpinBox();
  m_userPreferencesTab->enableMulticastCheckBox(0);
  m_userPreferencesTab->enableMulticastWidgets(0);
}


/*!
 */
void xpepedsWindow::showDisplayWindow()
{
  if (m_isWindowOpen) return;
  m_displayWindow = new pDisplayWindow();
  m_displayWindow -> show();
  connect(m_displayWindow, SIGNAL(windowClosed()),
          this, SLOT(displayClosed()));
  m_displayWindow -> showPedestals(m_pedRunController -> pedMap());
  m_isWindowOpen = true;
}


/*!
 */
void xpepedsWindow::closeDisplayWindow()
{
  if (!m_isWindowOpen) return;
  m_displayWindow -> close();
  displayClosed();
}


/*! Each time the window is closed, we reset the data.
 */
void xpepedsWindow::displayClosed()
{
  m_pedRunController -> reset();
  m_isWindowOpen = false;
}


/*!
 */
void xpepedsWindow::setupConnections()
{
  pAcquisitionWindow::setupConnections();
  connect (m_transportBar, SIGNAL(start()),
           this, SLOT(closeDisplayWindow()));
  connect (m_transportBar, SIGNAL(stop()),
           this, SLOT(showDisplayWindow()));
  connect (m_runController, SIGNAL(runStopped()),
           m_pedRunController, SLOT(writeMapToFile()));
}


/*!
 */
pDetectorConfiguration* xpepedsWindow::detectorConfiguration(int mode)
{
  if (mode == -1){
    mode = visualizationMode();
  }  
  pDetectorConfiguration *configuration = new pDetectorConfiguration();
  // Pedestals are always read in full frame
  configuration->setReadoutMode(xpoldetector::kFullFrameReadoutCode);
  // These are irrelevant (never used)
  configuration->setBufferMode(xpoldetector::kSmallBufferMode);
  configuration->setCalibrationDac(0);
  configuration->setPixelAddressX(0);
  configuration->setPixelAddressY(0);
  
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
  return configuration;
}
