#ifndef PLOTGRID_H
#define PLOTGRID_H

#include <iostream>

#include <QGridLayout>
#include <QSize>

#include "pHistogramOptions.h"
#include "pHistogramPlot.h"
#include "pHistogram.h"
#include "pEventDisplay.h"
#include "pMapPlot.h"
#include "pMap.h"
#include "xpemonPlotOptions.h"

class xpemonPlotGrid: public QWidget
{
  
  Q_OBJECT
  
  public:
  
    explicit xpemonPlotGrid(QWidget *parent = 0);
    // Sets the preferred initial size
    QSize sizeHint() const {return QSize(800, 800);}
    
  public:
  
    pHistogram* pulseHeightHist() {return m_pulseHeightHist;}
    pHistogram* windowSizeHist() {return m_windowSizeHist;}
    pMap* hitMap() {return m_hitMap;}
  
  public slots:
    
    void updatePulseHeightPlot();
    void updateWindowSizePlot();
    void updateHitMapPlot();
    void updateEventDisplay(unsigned int xmin, unsigned int xmax,
                            unsigned int ymin, unsigned int ymax,
                            const std::vector<double>& displayValues);
    void resetPlot();
     
  private:
    
    void setupPulseHeightPlot();
    void setupWindowSizePlot();
    void setupHitMap();
    void setupEventDisplay();
    
    pHistogram* m_pulseHeightHist;
    pHistogram* m_windowSizeHist;
    pMap* m_hitMap;
    QGridLayout *m_PlotLayout;
    pHistogramPlot *m_pulseHeightPlot;
    pHistogramPlot *m_windowSizePlot;
    pMapPlot *m_hitMapPlot;
    pEventDisplay *m_eventDisplay;
        
};

#endif // PLOTGRID_H
