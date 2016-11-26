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


#include "xperegWindow.h"
#include <iostream>


xperegWindow::xperegWindow(xperegRunController &runController) :
  pQtMainWindowGui(),
  m_runController(&runController)
{
  QString title = "xpereg version " + QString(__XPEDAQ_VERSION__);
  setWindowTitle(title);
  setupDaqDisplay();
  setupMessageDisplay();
  setupLoggerConnections();
  setupTransportBar();
  setupTabWidget();
  xperegUserPreferences *preferences = m_runController->userPreferences();
  displayUserPreferences(preferences);
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
  m_runController->init();
  showMessage("Data acquisition system ready", 2000);
}


/*!
 */
void xperegWindow::displayUserPreferences(xperegUserPreferences *preferences)
{
  m_registerTab->displayUserPreferences(preferences);
  m_userPreferencesTab->displayUserPreferences(preferences);
}


/*!
 */
void xperegWindow::start()
{
  m_transportBar->pressStartButton();
}

/*!
 */
void xperegWindow::stop()
{
  m_transportBar->pressStopButton();
}


/*! 
 */
void xperegWindow::startRun()
{
  m_runController->setupRun();
  m_runController->setRunning();
}

/*! 
 */
void xperegWindow::stopRun()
{
  m_runController->setStopped();
}


/*!
 */
void xperegWindow::setupDaqDisplay()
{
  m_daqDisplay = new pDaqDisplay(m_centralWidget);
  m_daqDisplay->freezeSize(DISPLAYS_WIDTH, -1);
  m_mainGridLayout->addWidget(m_daqDisplay, 0, 0, Qt::AlignTop);
}


/*!
 */
void xperegWindow::setupMessageDisplay()
{
  m_messageDisplay = new pMessageDisplay(m_centralWidget);
  m_messageDisplay->freezeSize(DISPLAYS_WIDTH, 150);
  m_mainGridLayout->addWidget(m_messageDisplay, 1, 0, Qt::AlignCenter);
}

/*!
 */
void xperegWindow::setupTransportBar()
{
  m_transportBar   = new pTransportBar(m_centralWidget);
  m_mainGridLayout->addWidget(m_transportBar, 3, 0, Qt::AlignBottom);
}


/*!
 */
void xperegWindow::setupTabWidget()
{
  m_mainTabWidget = new QTabWidget(m_centralWidget);
  m_mainGridLayout->addWidget(m_mainTabWidget, 0, 1, 2, 1);
  m_registerTab = new xperegRegisterTab();
  m_mainTabWidget->addTab(m_registerTab, "Registers"); 
  m_usbControlTab = new pUsbControlTab(false);
  m_mainTabWidget->addTab(m_usbControlTab, "USB");
  m_userPreferencesTab = new xperegUserPreferencesTab();
  m_mainTabWidget->addTab(m_userPreferencesTab, "Preferences");
}


void xperegWindow::disableHardwareWidgets()
{
  //m_transportBar->setEnabled(0);
}


void xperegWindow::disableTabs()
{
  m_registerTab->setEnabled(0);
  m_usbControlTab->setEnabled(0);
  m_userPreferencesTab->setEnabled(0);
}


/*!
*/
void xperegWindow::enableTabs()
{
  m_registerTab->setEnabled(1);
  m_usbControlTab->setEnabled(1);
  m_userPreferencesTab->setEnabled(1);
}


/*!
 */
void xperegWindow::setupConnections()
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
  //connect(m_runController, SIGNAL(numDataBlocksChanged(int)),
  //	  m_daqDisplay, SLOT(updateNumDataBlocks(int)));
  //connect(m_runController, SIGNAL(numEventsChanged(int)),
  //	  m_daqDisplay, SLOT(updateNumEvents(int)));
  connect(m_runController, SIGNAL(elapsedSecondsChanged(int)),
  	  m_daqDisplay, SLOT(updateElapsedSeconds(int)));
  //connect(m_runController, SIGNAL(averageEventRateChanged(double)),
  //	  m_daqDisplay, SLOT(updateAverageDaqRate(double)));
  //connect(m_runController, SIGNAL(instantEventRateChanged(double)),
  //m_daqDisplay, SLOT(updateInstantDaqRate(double)));
}


/*!
 */
void xperegWindow::setupLoggerConnections()
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


pDetectorConfiguration* xperegWindow::detectorConfiguration(int mode)
{ 
  pDetectorConfiguration *configuration = new pDetectorConfiguration();
  return configuration;
}
