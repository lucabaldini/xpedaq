#ifndef XPEMONWINDOW_H
#define XPEMONWINDOW_H

#include <iostream>

#include <QMainWindow>
#include <QGridLayout>
#include <QWidget>
#include <QTimer>
#include <QUdpSocket>

#include "xpemonSocketPortWidget.h"
#include "xpemonPlotGrid.h"
#include "pTransportBar.h"
#include "qcustomplot.h"
#include "pEventReader.h"

class xpemonWindow : public QMainWindow
{
  
  Q_OBJECT
  
  public:   

    explicit xpemonWindow(QWidget *parent = 0);
    
  private slots:
  
    void startRun();
    void stopRun();
    void sendDatagram();
  
  protected:
  
    QWidget* m_centralWidget;
    QGridLayout* m_mainGridLayout;
    xpemonPlotGrid* m_plotGrid;
    pTransportBar* m_transportBar;
    xpemonSocketPortWidget* m_socketPortWidget;
    pEventReader* m_eventReader;
    QTimer m_refreshTimer;
    
    //debug
    QUdpSocket testSenderSocket;
};

#endif // XPEMONWINDOW_H
