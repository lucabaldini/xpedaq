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

#ifndef PMONITORTAB_H
#define PMONITORTAB_H

#include <iostream>
#include <QString>
#include "pHistogram.h"
#include "pHistogramPlot.h"
#include "pMap.h"
#include "pMapPlot.h"
#include "pHistogramOptions.h"
#include "xpemonPlotOptions.h"
#include "pQtCustomTab.h"
#include "xpolgui.h"
#include "xpoldetector.h"

class pMonitorTab : public pQtCustomTab
{

  Q_OBJECT
  
  public:
  
    pMonitorTab();
    ~pMonitorTab() {;}
    // Sets the preferred initial size
    //QSize sizeHint() const {return QSize(800, 800);}
    
  public:
  
    pHistogram* pulseHeightHist() {return m_pulseHeightHist;}
    pHistogram* windowSizeHist() {return m_windowSizeHist;}
    pMap* hitMap() {return m_hitMap;}
  
  public slots:
    
    void updatePulseHeightPlot();
    void updateWindowSizePlot();
    void updateHitMapPlot();
    void resetPlot();
     
  private:
    
    void setupPulseHeightPlot();
    void setupWindowSizePlot();
    void setupHitMap();
    
    pHistogram* m_pulseHeightHist;
    pHistogram* m_windowSizeHist;
    pMap* m_hitMap;
    pHistogramPlot *m_pulseHeightPlot;
    pHistogramPlot *m_windowSizePlot;
    pMapPlot *m_hitMapPlot;
};

#endif //PMONITORTAB_H
