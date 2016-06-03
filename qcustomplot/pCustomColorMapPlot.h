#ifndef CUSTOMCOLORMAPPLOT_H
#define CUSTOMCOLORMAPPLOT_H

#include "qcustomplot.h"
#include "pHistogramOptions.h"
#include <iostream>

class pCustomColorMapPlot : public QCustomPlot
{
  
  Q_OBJECT
  
  public:
  
    pCustomColorMapPlot(pColorMapOptions options = pColorMapOptions());  
    
    void setCellContent(unsigned int xCell, unsigned int yCell, double value);
      
    void setDataContent(double x, double y, double value);
    
    void setRange (double xmin, double xmax, double ymin, double ymax);

    void rescaleDataRange() {m_colorMap -> rescaleDataRange();}
    
    void setSize  (unsigned int nXbins, unsigned int nYbins)
      {m_data -> setSize(nXbins, nYbins);}

    void clearMap();

  private slots:
  
    void mousePress();
    void mouseWheel();
    void selectionChanged();
      
  private:
    
    void setupInteractions();
   
    QCPColorMap *m_colorMap;
    QCPColorScale *m_colorScale;
    QCPMarginGroup *m_marginGroup;
    QCPColorMapData *m_data;  
    pColorMapOptions m_options;

};

#endif  //CUSTOMCOLORMAPPLOT_H
