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

#include "xpemonWindow.h"

xpemonWindow::xpemonWindow(std::string preferencesFilePath,
                           QWidget *parent) :
  QMainWindow(parent),
  m_preferencesFilePath (preferencesFilePath),
  m_isStopped(true)
{
  QString title = "xpemon version " + QString(__XPEDAQ_VERSION__);
  setWindowTitle(title);
  const int pixelFromScreenLeft = 10;
  const int pixelFromScreenTop = 10;
  const int pixelWidth = 1150;
  const int pixelHeight = 750;
  setGeometry(pixelFromScreenLeft, pixelFromScreenTop,
              pixelWidth, pixelHeight);
  m_centralWidget   = new QWidget();
  setCentralWidget(m_centralWidget);
  m_mainGridLayout  = new QGridLayout(m_centralWidget);

  //Read last used preferences from file
  m_preferences = new pMonitorPreferences(m_preferencesFilePath); 
  //Initialize option box with last used preferences
  m_optionBoxWidget = new pOptionBoxWidget(*m_preferences);
  m_mainGridLayout->addWidget(m_optionBoxWidget, 0, 0);
  //Cuts
  m_cutBoxWidget = new pCutBoxWidget(*m_preferences);
  if (m_preferences->m_showCuts) {
    m_mainGridLayout->addWidget(m_cutBoxWidget,
				m_mainGridLayout->rowCount(), 0);
  }
  //Info
  m_infoBoxWidget = new pInfoBoxWidget(this);
  m_mainGridLayout->addWidget(m_infoBoxWidget, m_mainGridLayout->rowCount(), 0);
  //Initialize the transport bar
  m_transportBar = new pTransportBar(this, false);
  m_mainGridLayout->addWidget(m_transportBar,
			      m_mainGridLayout->rowCount() + 1, 0);
  //Initialize the tabs
  m_mainTabWidget = new QTabWidget(m_centralWidget);
  m_mainGridLayout->addWidget(m_mainTabWidget, 0, 1, 
                                            m_mainGridLayout->rowCount(), 1);
  m_eventDisplayTab = new pEventDisplayTab();
  m_mainTabWidget->addTab(m_eventDisplayTab, "Event Display");
  m_monitorTab = new pMonitorTab();
  m_mainTabWidget->addTab(m_monitorTab, "Monitor Plots");
  //m_hitmapTab = new pHitmapTab();
  //m_mainTabWidget->addTab(m_hitmapTab, "Hit Map");
  m_mainGridLayout->setColumnStretch(1, 12);
  //Initialize the event reader
  m_eventReader = new pEventReader((*m_preferences),
				   m_monitorTab->windowSizeHist(),
				   m_monitorTab->clusterSizeHist(),
                                   m_monitorTab->pulseHeightHist(),
                                   m_monitorTab->modulationHist(),
                                   m_monitorTab->hitmap());
  
  setupConnections();
  // Enable the recon check boxes (this might go in the configuration file).
  m_infoBoxWidget->checkCheckBoxes(true);
  statusBar()->showMessage("Monitor system ready", 2000);
}


void xpemonWindow::readOptions()
{
  // Read options from the option boxes
  m_optionBoxWidget->options(m_preferences);
  m_cutBoxWidget->options(m_preferences);
}


void xpemonWindow::setupConnections()
{
  setupTransportBarConnections();
  connect(&m_refreshTimer, SIGNAL(timeout()),
          m_eventReader, SLOT(updateRequested()));
  connect(&m_refreshTimer, SIGNAL(timeout()),
          this, SLOT(showReaderStatMessage()));
  connect(this, SIGNAL(startAcquisition()),
          m_eventReader, SLOT(startReading()));
  connect(m_infoBoxWidget->drawFirstPassCheckBox(),
	  SIGNAL(stateChanged(int)),
	  m_eventDisplayTab->eventDisplay(),
	  SLOT(setFirstPassDisplayEnabled(int)));
  connect(m_infoBoxWidget->drawSearchRegionCheckBox(),
	  SIGNAL(stateChanged(int)),
	  m_eventDisplayTab->eventDisplay(),
	  SLOT(setSearchRegionDisplayEnabled(int)));
  connect(m_infoBoxWidget->drawSecondPassCheckBox(),
	  SIGNAL(stateChanged(int)),
	  m_eventDisplayTab->eventDisplay(),
	  SLOT(setSecondPassDisplayEnabled(int)));
  setupEvtReaderConnections();
}


void xpemonWindow::setupTransportBarConnections()
{
  connect(m_transportBar, SIGNAL(start()), this, SLOT(startRun()));
  connect(m_transportBar, SIGNAL(start()), m_optionBoxWidget, SLOT(disable()));
  connect(m_transportBar, SIGNAL(start()), m_cutBoxWidget, SLOT(disable()));
  
  connect(m_transportBar, SIGNAL(reset()), this, SLOT(reset()));
  connect(m_transportBar, SIGNAL(reset()), m_infoBoxWidget, SLOT(reset()));
  
  /* The stop button sends to the socket thread a stop request and stops the
     timer. A last refresh is executed, to make sure all the plots are 
     synchronized with the data on the socket thread. */
  connect(m_transportBar, SIGNAL(stop()), &m_refreshTimer, SLOT(stop())); 
  connect(m_transportBar, SIGNAL(stop()),
	  m_eventReader, SLOT(updateRequested()));
  connect(m_transportBar, SIGNAL(stop()), m_eventReader, SLOT(setStopped()));
  connect(m_transportBar, SIGNAL(stop()), m_optionBoxWidget, SLOT(enable()));
  connect(m_transportBar, SIGNAL(stop()), m_cutBoxWidget, SLOT(enable()));

  /* The pause button suspends the refresh of the plots, but continue the
     acquisition of data */
  connect(m_transportBar, SIGNAL(pause()), &m_refreshTimer, SLOT(stop()));
}


void xpemonWindow::setupEvtReaderConnections()
{
  connect(m_eventReader, SIGNAL(stopped()), this, SLOT(stopRun()));
  // Allow to pass events through signal/slot connection
  qRegisterMetaType< pEvent >("pEvent"); 
  // When the eventReader is ready, get the last event from it
  connect(m_eventReader, SIGNAL(lastEventUpdated(const pEvent&)),
          this, SLOT(showLastEvent(const pEvent&)));  
  // Update the other plots
  connect (m_eventReader, SIGNAL(histogramsUpdated(double, double)),
           m_monitorTab, SLOT(update(double, double)));
  //connect (m_eventReader, SIGNAL(histogramsUpdated()),
  //         m_hitmapTab, SLOT(update()));
}


void xpemonWindow::startRun()
{
  if (m_isStopped)
  /* If the monitor was stopped, we need to restart the socket thread */
  {
    readOptions();
    m_preferences->writeToFile(m_preferencesFilePath);
    m_eventReader->updatePreferences(*m_preferences);
    m_eventReader->moveToThread(&m_thread);
    m_thread.start();
    emit (startAcquisition());
    m_isStopped = false;
  }  
  /* If the monitor was paused and not stopped, we just need to reactivate the
     timer controlling the refresh of the plots */
  m_refreshTimer.start(m_preferences->m_refreshInterval);
}


void xpemonWindow::stopRun()
{
  m_thread.quit();
  m_thread.wait();
  m_isStopped = true;
}


void xpemonWindow::showLastEvent(const pEvent& evt)
{
  // Update event info and send last event to the event diplay
  m_infoBoxWidget->updateTime(evt.seconds());
  m_infoBoxWidget->updateAbsorptionPoint(evt.moma2().x0(), evt.moma2().y0());
  m_infoBoxWidget->updatePulseHeight(evt.pulseHeight());
  m_infoBoxWidget->updatePhi(evt.phiDeg());  

  m_infoBoxWidget->updateWindowSize(evt.firstCol(), evt.lastCol(),
                                    evt.firstRow(), evt.lastRow());
  int row, col;
  evt.highestPixelCoordinates(col, row);
  m_infoBoxWidget->updateMaxCoordinates(col, row);
  m_infoBoxWidget->updateClusterSize(evt.clusterSize());
  m_infoBoxWidget->updateBaricenterCoordinates(evt.moma1().x0(),
                                               evt.moma1().y0());
  m_infoBoxWidget->updateMom2Trans(evt.moma1().mom2trans());
  m_infoBoxWidget->updateMom2Long(evt.moma1().mom2long());
  m_eventDisplayTab->update(evt);  
  m_infoBoxWidget->updateMomRatio(evt.moma1().mom2long() /
                                  evt.moma1().mom2trans());
  m_infoBoxWidget->updateSkewness(evt.moma1().skewness());
}


/*! Refresh the event reader statistics on the status bar.
 */
void xpemonWindow::showReaderStatMessage()
{
  int numEventsAccepted = m_eventReader->numEventsAccepted();
  int numEventsRead = m_eventReader->numEventsRead();
  long int runningSeconds = m_eventReader->runningSeconds();
  std::pair<double, double> visibility =
    m_eventReader->stokesAccumulator()->visibility();
  std::pair<double, double> phase =
    m_eventReader->stokesAccumulator()->phaseDeg();
  if (runningSeconds > 0 && numEventsRead > 0) {
    double averageRate = numEventsRead/(double)runningSeconds;
    QString msg = QString::number(numEventsRead) + " event(s) read @ " +
      QString::number(averageRate, 'f', 2) + " Hz average rate";
    // If the preferences are setup to display the group box with the cuts, then
    // show the cut efficiency as well.
    if (m_preferences->m_showCuts) {
      double cutEfficiency = (100.*numEventsAccepted)/numEventsRead;
      msg += ", " + QString::number(numEventsAccepted) + " events accepted (" +
	QString::number(cutEfficiency, 'f', 1) + " % cut efficiency)";
    }
    // And, finally, the modulation.
    msg += ". Modulation = (" +
      QString::number(100*(visibility.first), 'f', 2) +
      " +/- " + QString::number(100*(visibility.second), 'f', 2) +
      ") %, phase = (" + QString::number(phase.first, 'f', 1) + " +/- " +
      QString::number(phase.second, 'f', 1) + ") degrees";
    statusBar()->showMessage(msg);
  }
}


void xpemonWindow::reset()
{
  m_eventDisplayTab->reset();
  m_monitorTab->reset();
  //m_hitmapTab->reset();
  statusBar()->showMessage("");
}
