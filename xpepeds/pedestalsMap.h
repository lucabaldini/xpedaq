#ifndef PEDESTALSMAP_H
#define PEDESTALSMAP_H

#include "qcustomplot.h"
#include "pMapOptions.h"
#include <vector>

/* Class implementing color map plots for pedestals (mean and value)*/


class PedestalsMap
{
  public:
    PedestalsMap(QCustomPlot* parentPlot, std::vector<double> const &values,
                 pMapOptions* options);
    
  private:

    pMapOptions *m_options;
    QCPColorMap *m_colorMap;
    QCPColorScale *m_colorScale;
    QCPMarginGroup *m_marginGroup;
};

#endif // PEDESTALSMAP_H
