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
#include <QStatusBar>

#include "pOptionBoxWidget.h"
#include "pCutBoxWidget.h"
#include "pEventDisplayTab.h"
#include "pMonitorTab.h"
#include "pHitmapTab.h"
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
    void readOptions();
    void showLastEvent(const pEvent& evt);
    void showReaderStatMessage();
    void reset();
  
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
    /* Tab containing the hit map */
    pHitmapTab* m_hitmapTab;
    /* Transport bar */
    pTransportBar* m_transportBar;  
    /* Widget showing the options that can be set by the user */ 
    pOptionBoxWidget* m_optionBoxWidget;
    /* Widget showing the cuts that can be set by the user */ 
    pCutBoxWidget* m_cutBoxWidget;
    /* Widget showing some event info */  
    pInfoBoxWidget* m_infoBoxWidget;

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
