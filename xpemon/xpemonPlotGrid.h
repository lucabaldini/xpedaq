#ifndef PLOTGRID_H
#define PLOTGRID_H

#include <iostream>

#include <QGridLayout>
#include <QSize>

#include "xpoldetector.h"
#include "pHistogramOptions.h"
#include "pHistogramPlot.h"
#include "pMapPlot.h"

class xpemonPlotGrid: public QWidget
{
  
  Q_OBJECT
  
  public:
  
    explicit xpemonPlotGrid(QWidget *parent = 0);
    QSize sizeHint() const {return QSize(800, 800);}
    
  public slots:
    
    void refreshPlot();
    void resetPlot();

  private slots:
    
    void fillPulseHeight(int pHeight);
    void addBarycenterPoint(double xBar, double yBar);
    void addHitMapPoint(double x, double y, unsigned int counts);
     
  private:
    
    QGridLayout *m_PlotLayout;
    pHistogramPlot *m_pulseHeightPlot;
    pMapPlot *m_barycenterPlot;
    pMapPlot *m_hitMap;
        
};

#endif // PLOTGRID_H
