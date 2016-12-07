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

#ifndef PEDVIEWERWINDOW_H
#define PEDVIEWERWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QThread>
#include <QMutex>
#include <QTabWidget>

#include "pedestalsMap.h"
#include "pedviewerPlotGrid.h"
#include "pedFile.h"
#include "pedmapFile.h"
#include "pedDataFile.h"
#include "pedviewerMenuBar.h"
#include "pQtCustomLineEdit.h"
#include "pQtCustomTextLabel.h"


/* Main window for the pedestal files viewer application. */


class pedviewerWindow : public QMainWindow
{
  Q_OBJECT
  
  public:
  
    explicit pedviewerWindow(QWidget *parent = 0, int windowWidth = 1080,
                             int windowWeight = 700);
  signals:
  
    void windowClosed();
        
  private slots:

    void openDataFile(const QString& filePath);
    void openMapFile(const QString& filePath);
    void loadReferenceFile(const QString& filePath); 
    void showPedestals();
    void showPedestalsWithRef();
    void showEvent(int evtNumber);
    void showMap();
    void nextPressed();
    void prevPressed();
    void evtNumberEditChanged();    
    void updatePlots();
    void updateNavBarStatus(int curEvent);
    void setButtonsEnabled(bool enabled);
    void setNavBarEnabled(bool enabled);
    void setTotEvtLabel(int numEvents);
    void updateEvtNumberEdit(int curEvent);
    void setEvtNumberEditRange(int min, int max);       
    void closeEvent(QCloseEvent *event);
  
  private:
  
    void setupNavBar();
    void setupConnections();
  
    /*** Graphical stuff ***/
    int m_windowHeight;
    int m_windowWidth;    
    QWidget *m_centralWidget;
    QVBoxLayout *m_verticalLayout;
    pedviewerPlotGrid *m_plotGrid;
    PedviewerMenuBar *m_menuBar;
    /*** Navigation bar ***/
    QHBoxLayout *m_navLayout;
    QPushButton *m_prevButton;
    QPushButton *m_nextButton;
    pQtCustomTextLabel *m_totEvtLabel;
    pQtCustomLineEdit<int> *m_evtNumberEdit; 
    QCheckBox *m_subtractRefCheckBox;
    
    /*** Data stuff ***/
    PedFile* m_inputFile;
    PedestalsMap* m_pedMap;
    PedestalsMap* m_referenceMap;
    int m_nEvents;
    int m_curEvent;
};

#endif // PEDVIEWERWINDOW_H
