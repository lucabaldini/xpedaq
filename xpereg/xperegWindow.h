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


#ifndef XPEREGWINDOW_H
#define XPEREGWINDOW_H

#include "pQtMainWindowGui.h"
#include "xperegRunController.h"
#include "pDaqDisplay.h"
#include "pDataDisplayWidget.h"
#include "pMessageDisplay.h"
#include "pTransportBar.h"
#include "pAcquisitionWindow.h"
#include "xperegRegisterTab.h"
#include "pUsbControlTab.h"
#include "xperegUserPreferencesTab.h"
#include "xperegUserPreferences.h"


class xperegWindow : public pQtMainWindowGui
{

  Q_OBJECT

 public:
  
  xperegWindow(xperegRunController &runController);
  ~xperegWindow() {;}

  virtual pDetectorConfiguration *detectorConfiguration(int mode = -1);
  xperegUserPreferences *userPreferences();
  
 public slots:
   
   void start();
   void stop();
   void startRun();
   void stopRun();
   void enableTabs();
   void disableTabs();
   void disableHardwareWidgets();
   void displayUserPreferences(xperegUserPreferences *preferences);
 
 private slots:
 
 private:
  
  pDaqDisplay *m_daqDisplay;
  pDataDisplayWidget *m_display;
  pMessageDisplay *m_messageDisplay;
  pTransportBar *m_transportBar;
  QTabWidget *m_mainTabWidget;
  xperegRegisterTab *m_registerTab;
  pUsbControlTab *m_usbControlTab;
  xperegUserPreferencesTab *m_userPreferencesTab;
  void setupDaqDisplay();
  void setupMessageDisplay();
  void setupTransportBar();
  void setupTabWidget();
  void setupLoggerConnections();
  xperegRunController *m_runController;
  void setupConnections();
};

#endif //XPEREGWINDOW_H
