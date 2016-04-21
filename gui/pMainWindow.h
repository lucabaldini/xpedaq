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
#include "pQtCustomTabWidget.h"
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
  pDetectorConfiguration *getConfiguration(int mode = -1);
  pUserPreferences *getUserPreferences();
  int getVisualizationMode();
  void displayConfiguration(pDetectorConfiguration *configuration, int mode);
  void disableHardware();
  void start();
  void stop();
  inline pRunController *getRunController()
    {return m_runController;}
  QString currentOutputFolder();

 protected:

 public slots:
  void selectConfiguration();
  void saveConfiguration(bool promptDialog = true);
  void saveUserPreferences();
  void changeVisualizationMode(int mode);
  void displayReference(unsigned short reference);

 private:
  pDaqDisplay *m_daqDisplay;
  pMessageDisplay *m_messageDisplay;
  pTransportBar *m_transportBar;
  pQtCustomTabWidget *m_mainTabWidget;
  pReadoutModeTab *m_readoutModeTab;
  pThresholdSettingTab *m_thresholdSettingTab;
  pAdvancedSettingsTab *m_advancedSettingsTab;
  pTriggerSettingTab *m_triggerSettingTab;
  pProbeSettingTab *m_probesSettingTab;
  pUsbControlTab *m_usbControlTab;
  pUserPreferencesTab *m_userPreferencesTab;
  pRunController *m_runController;
  QAction *m_selectConfigurationAction;
  QAction *m_saveConfigurationAction;
  QAction *m_savePreferencesAction;
  int m_lastVisualizationMode;
  std::string m_preferencesCfgFilePath;
  std::string m_detectorCfgFilePath;
  void setupDaqDisplay();
  void setupMessageDisplay();
  void setupTransportBar();
  void setupTabWidget();
  void setupActions();
  void setupMenuBar();
  void setupConnections();
  void setupLoggerConnections();
  void selectConfiguration(std::string filePath);
};

#endif
