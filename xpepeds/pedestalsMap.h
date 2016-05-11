#ifndef PEDESTALSMAP_H
#define PEDESTALSMAP_H

#include <array>
#include <memory>
#include "xpoldetector.h"
#include "qcustomplot.h"
#include "pMapOptions.h"

namespace pedestals{
  const int kNx = static_cast<int>(xpoldetector::kNumPixelsX);
  const int kNy = static_cast<int>(xpoldetector::kNumPixelsY);
  const int kNPedestal = 105600; //kNx*kNy;
  typedef std::array<double, kNPedestal> arr_t;
}


class PedestalsMapData : public QCPColorMapData
{ 
/* Small wrapper around QCPColorMapData class specific for pedestals data (i.e
   fix size to 352x300 and axis range accordingly).
*/

  public:
  
    PedestalsMapData(pedestals::arr_t const &data);

};


class PedestalsMap
{
/* Class implementing color map plots for pedestals (mean or rms) */

  public:
    PedestalsMap(QCustomPlot* parentPlot, PedestalsMapData* data,
                  std::shared_ptr<pColorMapOptions> options);
    ~PedestalsMap();
    
  private:

    QCPColorMap *m_colorMap;
    QCPColorScale *m_colorScale;
    QCPMarginGroup *m_marginGroup;
    
    // Not owned by the object. May live after the object is destroyed
    std::shared_ptr<pColorMapOptions> m_options;
         
};

#endif // PEDESTALSMAP_H
