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

#ifndef PMAINWINDOW_H
#define PMAINWINDOW_H

#include <iostream>

#include <QFileDialog>
#include <QDir>

#include "xpollog.h"
#include "xpolenv.h"
#include "pQtMainWindowGui.h"
#include "pDaqDisplay.h"
#include "pMessageDisplay.h"
#include "pTransportBar.h"
#include "pReadoutModeTab.h"
#include "pThresholdSettingTab.h"
#include "pAdvancedSettingsTab.h"
#include "pTriggerSettingTab.h"
#include "pProbeSettingTab.h"
#include "pUsbControlTab.h"
#include "pUserPreferencesTab.h"
#include "pRunController.h"
#include "pDetectorConfiguration.h"
#include "pUserPreferences.h"

class pRunController;

namespace gui{
  const int DISPLAYS_WIDTH = 560;
}

using namespace gui;


class pMainWindow : public pQtMainWindowGui
{

  Q_OBJECT

 public:
  
  pMainWindow();
  ~pMainWindow();
  pDetectorConfiguration *detectorConfiguration(int mode = -1);
  pUserPreferences *userPreferences();
  int visualizationMode();
  void displayConfiguration(pDetectorConfiguration *configuration, int mode);
  void start();
  void stop();
  inline pRunController *runController() const {return m_runController;}
  

 protected:

  
 public slots:
   
  void disableHardwareWidgets();
  void changeVisualizationMode(int mode);
  void displayReference(unsigned short reference);
  void startRun();
  void stopRun();

   
 private:
   
  pDaqDisplay *m_daqDisplay;
  pMessageDisplay *m_messageDisplay;
  pTransportBar *m_transportBar;
  QTabWidget *m_mainTabWidget;
  pReadoutModeTab *m_readoutModeTab;
  pThresholdSettingTab *m_thresholdSettingTab;
  pAdvancedSettingsTab *m_advancedSettingsTab;
  pTriggerSettingTab *m_triggerSettingTab;
  pProbeSettingTab *m_probesSettingTab;
  pUsbControlTab *m_usbControlTab;
  pUserPreferencesTab *m_userPreferencesTab;
  pRunController *m_runController;
  int m_lastVisualizationMode;
  void setupDaqDisplay();
  void setupMessageDisplay();
  void setupTransportBar();
  void setupTabWidget();
  void setupConnections();
  void setupLoggerConnections();
};

#endif //PMAINWINDOW_H
