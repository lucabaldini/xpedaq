#ifndef XPEMONWINDOW_H
#define XPEMONWINDOW_H

#include <iostream>
#include <vector>

#include <QThread>
#include <QMainWindow>
#include <QGridLayout>
#include <QWidget>
#include <QTimer>
#include <QMetaType>

#include "pOptionBoxWidget.h"
#include "xpemonPlotGrid.h"
#include "pInfoBoxWidget.h"
#include "pTransportBar.h"
#include "pEventReader.h"
#include "pMonitorOptions.h"

class xpemonWindow : public QMainWindow
{
  
  Q_OBJECT
  
  public:   

    explicit xpemonWindow(QWidget *parent = 0);
    
  signals:
      
    void startAcquisition();
  
  private slots:
  
    void startRun();
    void stopRun();
    void setupConnections();
    void reset();
    void readOptions();
  
  private:
  
    void setupEvtReaderConnections();
    void setupTransportBarConnections();
  
    /* Widget that contains all the other widgets */
    QWidget* m_centralWidget;
    /* Main layout object */
    QGridLayout* m_mainGridLayout;
    /* Widget containings the plots */
    xpemonPlotGrid* m_plotGrid;
    /* Transport bar */
    pTransportBar* m_transportBar;  
    /* Widget showing the options that can be setted by the user */ 
    pOptionBoxWidget* m_optionBoxWidget;
    /* Widget showing some event info */  
    pInfoBoxWidget *m_infoBoxWidget;

    /* Data structure hoding the user-controlled options */
    pMonitorOptions m_options;

    /* Read the the data and fill histograms */ 
    pEventReader* m_eventReader;
    /* Thread where the event reader operates */
    QThread m_thread;
    /* Flag for signaling the event reader to reset the histograms*/
    bool m_isResetRequested;

    /* Timer controlling the refresh of the plot */
    QTimer m_refreshTimer;
    
};

#endif // XPEMONWINDOW_H
