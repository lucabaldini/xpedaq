#ifndef PLOTGRID_H
#define PLOTGRID_H

#include <iostream>

#include <QGridLayout>
#include <QSize>

#include "pHistogramOptions.h"
#include "pHistogramPlot.h"
#include "pCustomColorMapPlot.h"
#include "pMapPlot.h"
#include "xpemonPlotOptions.h"

class xpemonPlotGrid: public QWidget
{
  
  Q_OBJECT
  
  public:
  
    explicit xpemonPlotGrid(QWidget *parent = 0);
    // Sets the preferred initial size
    QSize sizeHint() const {return QSize(800, 800);}
    
  public slots:
    
    void updatePulseHeightPlot(const std::vector<double>& pulseHeightValues);
    void updateWindowSizePlot(const std::vector<double>& windowSizeValues);
    void updateHitMap(const std::vector<double>& hitMapValues);
    void updateEventDisplay(double xmin, double xmax, double ymin, double ymax,
                            const std::vector<double>& displayValues);
    void resetPlot();
     
  private:
    
    void setupPulseHeightPlot();
    void setupWindowSizePlot();
    void setupHitMap();
    void setupEventDisplay();
    
    QGridLayout *m_PlotLayout;
    pHistogramPlot *m_pulseHeightPlot;
    pHistogramPlot *m_windowSizePlot;
    pMapPlot *m_hitMap;
    pCustomColorMapPlot *m_eventDisplay;
        
};

#endif // PLOTGRID_H
