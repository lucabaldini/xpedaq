#ifndef PLOTGRID_H
#define PLOTGRID_H

#include <iostream>

#include <QGridLayout>
#include <QSize>

#include "pulseHeightPlot.h"
#include "barycenterPlot.h"
#include "pMapOptions.h"

class xpemonPlotGrid: public QWidget
{
  
  Q_OBJECT
  
  public:
  
    explicit xpemonPlotGrid(QWidget *parent = 0);
    QSize sizeHint() const {return QSize(800, 800);}
    
  public slots:
    
    void refreshPlot();

  private slots:
    
    void addPulseHeightPoint(int pHeight);
    void addBarycenterPoint(double xBar, double yBar);
     
  private:
    
    //void setupPulseHeightPlot();
    //void setupBarycenterPlot();   
    QGridLayout *m_PlotLayout;
    PulseHeightPlot *m_pulseHeightPlot;
    BarycenterPlot *m_barycenterPlot;
        
};

#endif // PLOTGRID_H
