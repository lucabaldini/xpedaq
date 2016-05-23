#include "xpemonWindow.h"
#include <iostream>

xpemonWindow::xpemonWindow(QWidget *parent) : QMainWindow(parent)
{
  this->setGeometry(20, 20, 1280, 900);
  m_centralWidget   = new QWidget;
  setCentralWidget(m_centralWidget);
  m_mainGridLayout  = new QGridLayout(m_centralWidget);

  m_socketPortWidget = new xpemonSocketPortWidget();
  m_mainGridLayout->addWidget(m_socketPortWidget, 0,0);  

  m_plotGrid = new xpemonPlotGrid();
  m_mainGridLayout->addWidget(m_plotGrid, 0,1,2,1);
  m_mainGridLayout->setColumnStretch(1, 10);
  
  m_transportBar = new pTransportBar(this);
  m_mainGridLayout->addWidget(m_transportBar, 1,0);
  
  m_eventReader = new pEventReader(50001);
  
  connect(m_transportBar, SIGNAL(start()), this, SLOT(startRun()));
  connect(m_transportBar, SIGNAL(stop()), this, SLOT(stopRun()));
  connect(m_transportBar, SIGNAL(stop()), &m_refreshTimer, SLOT(stop())); 
  connect(m_transportBar, SIGNAL(stop()), m_eventReader, SLOT(setStopped()));
  connect(&m_refreshTimer, SIGNAL(timeout()), m_plotGrid, SLOT(refreshPlot()));
  
  connect(m_eventReader, SIGNAL(pulseHeightRead(int)), 
          m_plotGrid, SLOT(addPulseHeightPoint(int)));
  connect(m_eventReader, SIGNAL(barycenterRead(double, double)), 
          m_plotGrid, SLOT(addBarycenterPoint(double, double)));          
  
  //Debug
  //connect(&m_refreshTimer, SIGNAL(timeout()), this, SLOT(sendDatagram()));
}


//Debug only function
void xpemonWindow::sendDatagram()
{
  static int counter = 0;
  QByteArray datagram;
  QDataStream out(&datagram, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_4_3);
  out << counter;
  testSenderSocket.writeDatagram(datagram, QHostAddress::LocalHost, 50001);
  counter++;
}


void xpemonWindow::startRun()
{
  m_refreshTimer.start(1000);
  m_eventReader->startReading();
  std::cout << "started" << std::endl;
  
}


void xpemonWindow::stopRun()
{
  std::cout << "stopped" << std::endl;
}
