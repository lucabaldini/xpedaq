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
#include "pModulationHistogram.h"
#include "pHistogramPlot.h"
#include "pMap.h"
#include "pHitmap.h"
#include "pHistogramOptions.h"
#include "xpemonPlotOptions.h"
#include "pQtCustomTab.h"
#include "xpolgui.h"
#include "xpoldetector.h"
#include "pStatBox.h"


class pMonitorTab : public pQtCustomTab
{

  Q_OBJECT
  
  public:
  
    pMonitorTab(bool showModulationPlot = true);
    ~pMonitorTab() {;}
    // Sets the preferred initial size
    //QSize sizeHint() const {return QSize(800, 800);}
    
  public:

    pHistogram* windowSizeHist() {return m_windowSizeHist;}
    pHistogram* clusterSizeHist() {return m_clusterSizeHist;}
    pHistogram* pulseHeightHist() {return m_pulseHeightHist;}
    pModulationHistogram* modulationHist() {return m_modulationHist;}
    pMap* hitmap() {return m_hitmap;}
    void updateModulationFit();
    void resetModulationFit();
    void updatePulseHeightFit();
    void resetPulseHeightFit();
  
  public slots:

    void update();
    void reset();
     
  private:

    bool m_showModulationPlot;

    void setupWindowSizePlot();
    void setupClusterSizePlot();
    void setupPulseHeightPlot();
    void setupModulationPlot();
    void setupHitmapPlot();

    pHistogram* m_windowSizeHist;
    pHistogram* m_clusterSizeHist;
    pHistogram* m_pulseHeightHist;    
    pModulationHistogram* m_modulationHist;
    pMap* m_hitmap;

    pHistogramPlot *m_windowSizePlot;
    pHistogramPlot *m_clusterSizePlot;
    pHistogramPlot *m_pulseHeightPlot;
    pHistogramPlot *m_modulationPlot;
    pHitmap *m_hitmapPlot;

    pStatBox *m_pulseHeightStatBox;
    pStatBox *m_modulationStatBox;
};

#endif //PMONITORTAB_H
