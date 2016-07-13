#ifndef PEDSPLOTGRID_H
#define PEDSPLOTGRID_H

#include <iostream>

#include <QGridLayout>
#include <QSize>

#include "pHistogramOptions.h"
#include "pHistogramPlot.h"
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
    
    QGridLayout *m_PlotLayout;
    pMapPlot *m_averageMap;
    pMapPlot *m_rmsMap;
    pHistogramPlot *m_averageHist;
    pHistogramPlot *m_rmsHist;
        
};


#endif // PEDSPLOTGRID_H
