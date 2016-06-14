#include "xpemonWindow.h"
#include <iostream>

xpemonWindow::xpemonWindow(QWidget *parent) : QMainWindow(parent),
                                              m_isResetRequested (false),
                                              m_isStopped(true)
{
  const int pixelFromScreenLeft = 20;
  const int pixelFromScreenTop = 20;
  const int pixelWidth = 1366;
  const int pixelHeight = 768;
  this->setGeometry(pixelFromScreenLeft, pixelFromScreenTop,
                    pixelWidth, pixelHeight);
  m_centralWidget   = new QWidget();
  setCentralWidget(m_centralWidget);
  m_mainGridLayout  = new QGridLayout(m_centralWidget);

  m_optionBoxWidget = new pOptionBoxWidget();
  const int optBoxRowStart = 0;
  const int optBoxColStart = 0;
  m_mainGridLayout->addWidget(m_optionBoxWidget, optBoxRowStart,
                                                 optBoxColStart);  

  readOptions();
  
  m_plotGrid = new xpemonPlotGrid();
  m_mainGridLayout->addWidget(m_plotGrid, 0, 1, 6, 1);
  
  m_transportBar = new pTransportBar(this, false);
  m_mainGridLayout->addWidget(m_transportBar, 5,0);
  
  m_eventReader = new pEventReader(m_options.m_socketPortNumber,
                                   m_options.m_zeroSupThreshold);
  
  m_infoBoxWidget = new pInfoBoxWidget();
  m_mainGridLayout->addWidget(m_infoBoxWidget, 2,0);
  
  m_mainGridLayout->setColumnStretch(1, 12);

  setupConnections();
}


void xpemonWindow::readOptions()
{
  //Get the options from the option boxes
  unsigned int socketPortNumber;
  double refreshTime;
  unsigned int zeroSupThreshold;
  m_optionBoxWidget -> options(socketPortNumber, refreshTime,
                               zeroSupThreshold);
  m_options = pMonitorOptions(refreshTime, socketPortNumber,
                              zeroSupThreshold);
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
                                                 
  connect(m_eventReader, SIGNAL(highestPixelFound(unsigned int, unsigned int)),
          m_infoBoxWidget, SLOT(updateMaxCoordinates(unsigned int,
                                                     unsigned int )));
  
  qRegisterMetaType< std::vector<double> >("std::vector<double>");
  
  connect (m_eventReader, SIGNAL(pulseHeightUpdated(const std::vector<double>&)),
           m_plotGrid, SLOT(updatePulseHeightPlot(const std::vector<double>&)));

  connect (m_eventReader, SIGNAL(windowSizeUpdated(const std::vector<double>&)),
           m_plotGrid, SLOT(updateWindowSizePlot(const std::vector<double>&)));
           
  connect (m_eventReader, SIGNAL(hitMapUpdated(const std::vector<double>&)),
           m_plotGrid, SLOT(updateHitMap(const std::vector<double>&)));
  
  connect (m_eventReader, SIGNAL(evtDisplayUpdated(double, double,
                                                   double, double,
                                                   const std::vector<double>&)),
           m_plotGrid, SLOT(updateEventDisplay(double, double,
                                               double, double,
                                               const std::vector<double>&)));                                                             
}


void xpemonWindow::startRun()
{
  if (m_isStopped)
  {
    readOptions();
    m_eventReader -> setSocketPortNumber(m_options.m_socketPortNumber);
    m_eventReader -> setZeroSupThreshold(m_options.m_zeroSupThreshold);
    /* If the reset has been scheduled, execute it now */
    if (m_isResetRequested) m_eventReader -> resetHistograms();
    m_isResetRequested = false;
  
    m_eventReader -> moveToThread(&m_thread);
    m_thread.start();
    emit (startAcquisition());
    m_isStopped = false;
  }
  
  /* If the monitor was paused and not stopped, we just need to reactivate the
     refresh of the plot */
  m_refreshTimer.start(m_options.m_refreshInterval);
  std::cout << "started" << std::endl;
}


void xpemonWindow::stopRun()
{
  m_thread.quit();
  m_thread.wait();
  m_isStopped = true;
  std::cout << "stopped" << std::endl;
}


void xpemonWindow::reset()
{
  m_plotGrid -> resetPlot();
  /* Since the reset can happen only when the socket thread is stopped, we
     delay the actual reset of the histogram in that thread to the next
     'start' pressure, using this flag.
  */
  m_isResetRequested = true;
}


/*~xpemonWindow::xpemonWindow()
{
  if (m_thread.isRunning())
  {
    stopRun()
  }
  
}
*/
