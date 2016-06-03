#ifndef PLOTGRID_H
#define PLOTGRID_H

#include <iostream>

#include <QGridLayout>
#include <QSize>
#include <QMutex>

#include "xpoldetector.h"
#include "pHistogramOptions.h"
#include "pHistogramPlot.h"
#include "pMapPlot.h"

class xpemonPlotGrid: public QWidget
{
  
  Q_OBJECT
  
  public:
  
    explicit xpemonPlotGrid(QWidget *parent = 0);
    // Sets the preferred initial size
    QSize sizeHint() const {return QSize(800, 800);}
    
  public slots:
    
    void refreshPlot();
    void resetPlot();

  private slots:
    
    void fillPulseHeight(unsigned int pHeight);
    void writeWindow(unsigned int xmin, unsigned int xmax,
                     unsigned int ymin, unsigned int ymax);
    void fillBarycenter(double xBar, double yBar);
    void writePoint(unsigned int x, unsigned int y, unsigned int counts);
     
  private:
    
    void setupPulseHeightPlot();
    void setupWindowSizePlot();
    void setupHitMap();
    void setupEventDisplay();
    
    QGridLayout *m_PlotLayout;
    pHistogramPlot *m_pulseHeightPlot;
    pHistogramPlot *m_windowSizePlot;
    pMapPlot *m_barycenterPlot; // not used at the moment
    pMapPlot *m_hitMap;
    pCustomColorMapPlot *m_eventDisplay;
        
};

#endif // PLOTGRID_H
