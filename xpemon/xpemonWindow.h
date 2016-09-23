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
#include "pEventDisplayTab.h"
#include "pMonitorTab.h"
#include "pInfoBoxWidget.h"
#include "pTransportBar.h"
#include "pEventReader.h"
#include "pMonitorPreferences.h"

class xpemonWindow : public QMainWindow
{
  
  Q_OBJECT
  
  public:   

    xpemonWindow(std::string preferencesFilePath,
                 QWidget *parent = 0);
    
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
    /* Widget containing all the tabs */
    QTabWidget *m_mainTabWidget;    
    /* Tab containing the event diplay */
    pEventDisplayTab *m_eventDisplayTab;
    /* Tab containing the incremental plots */
    pMonitorTab* m_monitorTab;
    /* Transport bar */
    pTransportBar* m_transportBar;  
    /* Widget showing the options that can be setted by the user */ 
    pOptionBoxWidget* m_optionBoxWidget;
    /* Widget showing some event info */  
    pInfoBoxWidget *m_infoBoxWidget;

    /* Path to the preferences file*/
    std::string m_preferencesFilePath;
    /* Class for handling the user-controlled options */
    pMonitorPreferences* m_preferences;

    /* Read the the data and fill histograms */ 
    pEventReader* m_eventReader;
    /* Thread where the event reader operates */
    QThread m_thread;
    /* Flag to differentiate a restart from a stop status and a restart from
       a pause status */
    bool m_isStopped;

    /* Timer controlling the refresh of the plot */
    QTimer m_refreshTimer;
    
};

#endif // XPEMONWINDOW_H
