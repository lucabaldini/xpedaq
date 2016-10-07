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
  const int pixelFromScreenLeft = 20;
  const int pixelFromScreenTop = 20;
  const int pixelWidth = 1130;
  const int pixelHeight = 750;
  setGeometry(pixelFromScreenLeft, pixelFromScreenTop,
                    pixelWidth, pixelHeight);
  m_centralWidget   = new QWidget();
  setCentralWidget(m_centralWidget);
  m_mainGridLayout  = new QGridLayout(m_centralWidget);

  m_preferences = new pMonitorPreferences(m_preferencesFilePath); 
  m_optionBoxWidget = new pOptionBoxWidget(
                        m_preferences -> socketPort(),
                        m_preferences -> refreshInterval(),
                        m_preferences -> zeroSuppressionThreshold());
  const int optBoxRowStart = 0;
  const int optBoxColStart = 0;
  m_mainGridLayout->addWidget(m_optionBoxWidget, optBoxRowStart,
                                                 optBoxColStart);
  
  m_mainTabWidget = new QTabWidget(m_centralWidget);
  m_mainGridLayout->addWidget(m_mainTabWidget, 0, 1, 6, 1);
  m_eventDisplayTab = new pEventDisplayTab();
  m_mainTabWidget->addTab(m_eventDisplayTab, "Event Display");
  m_monitorTab = new pMonitorTab();
  m_mainTabWidget->addTab(m_monitorTab, "Monitor Plots");
  
  //m_plotGrid = new xpemonPlotGrid();
  //m_mainGridLayout->addWidget(m_plotGrid, 0, 1, 6, 1);
  
  m_transportBar = new pTransportBar(this, false);
  m_mainGridLayout->addWidget(m_transportBar, 5,0);
  
  m_eventReader = new pEventReader(m_preferences -> socketPort(),
                    m_preferences -> zeroSuppressionThreshold(),
                    m_monitorTab -> pulseHeightHist(),
                    m_monitorTab -> windowSizeHist(),
                    m_monitorTab -> modulationHist(),
                    m_monitorTab -> hitMap());
  
  m_infoBoxWidget = new pInfoBoxWidget();
  m_mainGridLayout->addWidget(m_infoBoxWidget, 2,0);
  
  m_mainGridLayout->setColumnStretch(1, 12);

  setupConnections();
}


void xpemonWindow::readOptions()
{
  // Read options from the option boxes
  unsigned int socketPortNumber;
  double refreshTime;
  unsigned int zeroSupThreshold;
  m_optionBoxWidget -> options(socketPortNumber, refreshTime,
                               zeroSupThreshold);
  m_preferences -> setSocketPort(socketPortNumber);
  m_preferences -> setRefreshInterval(refreshTime);
  m_preferences -> setZeroSuppressionThreshold(zeroSupThreshold);
}


void xpemonWindow::setupConnections()
{
  setupTransportBarConnections();
  connect(&m_refreshTimer, SIGNAL(timeout()),
          m_eventReader, SLOT(updateRequested()));
  connect(this, SIGNAL(startAcquisition()),
          m_eventReader, SLOT(startReading()));
  setupEvtReaderConnections();            
}


void xpemonWindow::setupTransportBarConnections()
{
  connect(m_transportBar, SIGNAL(start()), this, SLOT(startRun()));
  connect(m_transportBar, SIGNAL(start()),
          m_optionBoxWidget, SLOT(disableWidgets()));
  
  connect(m_transportBar, SIGNAL(reset()), this, SLOT(reset()));
  connect(m_transportBar, SIGNAL(reset()), m_infoBoxWidget, SLOT(reset()));
  
  /* The stop button sends to the socket thread a stop request and stops the
     timer. A last refresh is executed, to make sure all the plots are 
     synchronized with the data on the socket thread. */
  connect(m_transportBar, SIGNAL(stop()), &m_refreshTimer, SLOT(stop())); 
  connect(m_transportBar, SIGNAL(stop()),
          m_eventReader, SLOT(updateRequested()));
  connect(m_transportBar, SIGNAL(stop()), m_eventReader, SLOT(setStopped()));
  connect(m_transportBar, SIGNAL(stop()),
          m_optionBoxWidget, SLOT(activateWidgets()));

  /* The pause button suspends the refresh of the plot, but continue the
     acquisition of data */
  connect(m_transportBar, SIGNAL(pause()), &m_refreshTimer, SLOT(stop()));
}


void xpemonWindow::setupEvtReaderConnections()
{
  connect(m_eventReader, SIGNAL(stopped()), this, SLOT(stopRun()));
                                           
  connect(m_eventReader, SIGNAL(eventRead(unsigned int, unsigned int,
                                          unsigned int, unsigned int)),
          m_infoBoxWidget, SLOT(updateCounter()));          
  connect(m_eventReader, SIGNAL(eventRead(unsigned int, unsigned int,
                                          unsigned int, unsigned int)),
          m_infoBoxWidget, SLOT(updateWindowSize(unsigned int, unsigned int,
                                                unsigned int, unsigned int)));
                                                 
  connect(m_eventReader, SIGNAL(highestPixelFound(double,
                                                  double)),
          m_infoBoxWidget, SLOT(updateMaxCoordinates(double,
                                                     double)));
  connect(m_eventReader, SIGNAL(barycenterRead(double, double)),
          m_infoBoxWidget, SLOT(updateBarycenterCoordinates(double,
                                                            double)));
  
  qRegisterMetaType< pEvent >("pEvent");
  
  connect (m_eventReader, SIGNAL(pulseHeightUpdated()),
           m_monitorTab, SLOT(updatePulseHeightPlot()));

  connect (m_eventReader, SIGNAL(windowSizeUpdated()),
           m_monitorTab, SLOT(updateWindowSizePlot()));
           
  connect (m_eventReader, SIGNAL(modulationUpdated()),
           m_monitorTab, SLOT(updateModulationPlot()));           
           
  connect (m_eventReader, SIGNAL(hitMapUpdated()),
           m_monitorTab, SLOT(updateHitMapPlot()));
  
  connect (m_eventReader, SIGNAL(evtDisplayUpdated(const pEvent&)),
           m_eventDisplayTab, SLOT(updateEventDisplay(const pEvent&)));                                                             
}


void xpemonWindow::startRun()
{
  if (m_isStopped)
  {
    readOptions();
    m_preferences -> writeToFile (m_preferencesFilePath);
    m_eventReader -> setSocketPortNumber(m_preferences -> socketPort());
    m_eventReader -> setZeroSupThreshold(m_preferences ->
                                          zeroSuppressionThreshold());  
    m_eventReader -> moveToThread(&m_thread);
    m_thread.start();
    emit (startAcquisition());
    m_isStopped = false;
  }  
  /* If the monitor was paused and not stopped, we just need to reactivate the
     timer controlling the refresh of the plots */
  m_refreshTimer.start(m_preferences ->refreshInterval());
}


void xpemonWindow::stopRun()
{
  m_thread.quit();
  m_thread.wait();
  m_isStopped = true;
}


void xpemonWindow::reset()
{
  m_monitorTab -> resetPlot();
  m_eventDisplayTab -> resetPlot();  
}
