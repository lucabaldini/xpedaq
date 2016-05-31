#ifndef XPEMONWINDOW_H
#define XPEMONWINDOW_H

#include <iostream>

#include <QThread>
#include <QMainWindow>
#include <QGridLayout>
#include <QWidget>
#include <QTimer>
#include <QUdpSocket>

#include "pOptionBoxWidget.h"
#include "xpemonPlotGrid.h"
#include "pTransportBar.h"
#include "pEventReader.h"
#include "pInfoBoxWidget.h"
#include "pMonitorOptions.h"

class xpemonWindow : public QMainWindow
{
  
  Q_OBJECT
  
  public:   

    explicit xpemonWindow(QWidget *parent = 0);
    //~xpemonWindow();
    
  signals:
      
    void startAcquisition();
  
  private slots:
  
    void startRun();
    void stopRun();
    //void sendDatagram();
    void setupConnections();
    void reset();
    void readOptions();
  
  private:
  
    void setupEvtReaderConnections();
    void setupTransportBarConnections();
  
    QWidget* m_centralWidget;
    QGridLayout* m_mainGridLayout;
    xpemonPlotGrid* m_plotGrid;
    pTransportBar* m_transportBar;
    pOptionBoxWidget* m_optionBoxWidget;
    pEventReader* m_eventReader;
    pInfoBoxWidget *m_infoBoxWidget;
    QTimer m_refreshTimer;
    QThread m_thread;
    pMonitorOptions m_options;
    
};

#endif // XPEMONWINDOW_H
