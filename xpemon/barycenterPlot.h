#ifndef BARYCENTERPLOT_H
#define BARYCENTERPLOT_H

#include "qcustomplot.h"
#include "pMapOptions.h"
#include "xpoldetector.h"

class BarycenterPlot: public QCustomPlot
{
  public:
    BarycenterPlot(pColorMapOptions* options);
    void addPoint(double xBar, double yBar);
    ~BarycenterPlot();
  
  private:
    QCPColorMap *m_barycenterMap;
    QCPColorScale *m_colorScale;
    QCPMarginGroup *m_marginGroup;
    QCPColorMapData *m_barycenterData;  
    pColorMapOptions *m_options;
};

#endif  //BARYCENTERPLOT_H
