#include "xpemonWindow.h"
#include <iostream>

xpemonWindow::xpemonWindow(QWidget *parent) : QMainWindow(parent)
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
  connect(&m_refreshTimer, SIGNAL(timeout()), m_plotGrid, SLOT(refreshPlot()));
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
  
  connect(m_transportBar, SIGNAL(stop()), &m_refreshTimer, SLOT(stop())); 
  connect(m_transportBar, SIGNAL(stop()), m_plotGrid, SLOT(refreshPlot()));
  connect(m_transportBar, SIGNAL(stop()), m_eventReader, SLOT(setStopped()));
  connect(m_transportBar, SIGNAL(stop()),
          m_optionBoxWidget, SLOT(activateWidgets()));

  connect(m_transportBar, SIGNAL(pause()), &m_refreshTimer, SLOT(stop()));
}


void xpemonWindow::setupEvtReaderConnections()
{
  connect(m_eventReader, SIGNAL(stopped()), this, SLOT(stopRun()));
    
  connect(m_eventReader, SIGNAL(eventRead(unsigned int, unsigned int,
                                          unsigned int, unsigned int)),
          m_plotGrid, SLOT(writeWindow(unsigned int, unsigned int,
                                       unsigned int, unsigned int)));                                          
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
                                                     
  connect(m_eventReader, SIGNAL(totPulseHeightRead(unsigned int)), 
          m_plotGrid, SLOT(fillPulseHeight(unsigned int)));
          
  connect(m_eventReader, SIGNAL(pulseHeightRead(unsigned int, unsigned int,
                                                              unsigned int)),
          m_plotGrid, SLOT(writePoint(unsigned int, unsigned int,
                                                    unsigned int)));
                                                    
  //connect(m_eventReader, SIGNAL(barycenterRead(double, double)), 
  //        m_plotGrid, SLOT(fillBarycenter(double, double)));                                                    
}


void xpemonWindow::startRun()
{
  readOptions();
  m_eventReader -> setSocketPortNumber(m_options.m_socketPortNumber);
  m_eventReader -> setZeroSupThreshold(m_options.m_zeroSupThreshold);
  m_refreshTimer.start(m_options.m_refreshInterval);
  m_eventReader -> moveToThread(&m_thread);
  m_thread.start();
  emit (startAcquisition());
  std::cout << "started" << std::endl;
}


void xpemonWindow::stopRun()
{
  m_thread.quit();
  m_thread.wait();
  std::cout << "stopped" << std::endl;
}


void xpemonWindow::reset()
{
  m_plotGrid -> resetPlot();
}


/*~xpemonWindow::xpemonWindow()
{
  if (m_thread.isRunning())
  {
    stopRun()
  }
  
}
*/
