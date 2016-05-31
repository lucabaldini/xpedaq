#ifndef MAPPLOT_H
#define MAPPLOT_H

#include "qcustomplot.h"
#include "pMap.h"
#include "pHistogramOptions.h"

class pMapPlot : public QCustomPlot
{
  
  Q_OBJECT
  
  public:
  
    pMapPlot(unsigned int nXbins, double xmin, double xmax,
             unsigned int nYbins, double ymin, double ymax,
             pColorMapOptions options = pColorMapOptions());
  
    unsigned int entries() const;
    double sum() const;
    void fill(double x, double y);
    void fill(double x, double y, double value);
    
    // changes the visualized range (does not change the underlying histogram)
    void setRange (unsigned int xmin, unsigned int xmax,
                   unsigned int ymin, unsigned int ymax);
    
    // clears data both from the color map and from the histogram.
    // resizes the color map to match the underlying histogram.
    // replot the histogram
    void reset();
    
    // resets the data in the underlying histogram and in the color map
    void resetData();
    
    // Terminal formatting.
    friend std::ostream& operator<<(std::ostream& os, const pMapPlot& mapPlot)
      {return mapPlot.m_map-> fillStream(os);}

  private slots:
  
    void mousePress();
    void mouseWheel();
    void selectionChanged();
      
  private:
    
    void setupInteractions();
    void setupDataMap();
    
    //Align histogram bins and color map cells in a given range 
    void setMacthingRange(unsigned int xmin, unsigned int xmax,
                          unsigned int ymin, unsigned int ymax);
    
    pMap *m_map;
    QCPColorMap *m_colorMap;
    QCPColorScale *m_colorScale;
    QCPMarginGroup *m_marginGroup;
    QCPColorMapData *m_data;  
    pColorMapOptions m_options;

};

#endif  //MAPPLOT_H
