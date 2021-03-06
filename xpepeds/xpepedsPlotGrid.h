#ifndef PEDSPLOTGRID_H
#define PEDSPLOTGRID_H

#include <iostream>

#include <QGridLayout>
#include <QSize>

#include "pHistogramOptions.h"
#include "pHistogramPlot.h"
#include "pHistogram.h"
#include "pMap.h"
#include "pMapPlot.h"
#include "pedestalsMap.h"

class xpepedsPlotGrid: public QWidget
{
  
  Q_OBJECT
  
  public:
  
    explicit xpepedsPlotGrid(QWidget *parent = 0);
    // Sets the preferred initial size
    QSize sizeHint() const {return QSize(800, 800);}
    
  public slots:
    
    void fillPlots(const PedestalsMap& pedMap);
    void replotAll();
     
  private:
    
    void setupAverageMap();
    void setupRmsMap();
    void setupAverageHist();
    void setupRmsHist();
    
    pMap *m_averageMap;
    pMap *m_rmsMap;
    pHistogram *m_averageHist;
    pHistogram *m_rmsHist;
    QGridLayout *m_PlotLayout;
    pMapPlot *m_averageMapPlot;
    pMapPlot *m_rmsMapPlot;
    pHistogramPlot *m_averagePlot;
    pHistogramPlot *m_rmsPlot;
        
};


#endif // PEDSPLOTGRID_H
