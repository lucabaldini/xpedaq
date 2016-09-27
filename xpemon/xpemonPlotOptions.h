#ifndef XPEMONPLOTOPTIONS_H
#define XPEMONPLOTOPTIONS_H

#include "xpoldetector.h"
#include "qcustomplot.h"

namespace xpemonPlotOptions
{
  extern const unsigned int pulseHeightNbins;
  extern const double pulseHeightXmin;
  extern const double pulseHeightXmax;
  extern const unsigned int windowSizeNbins;
  extern const double windowSizeXmin;
  extern const double windowSizeXmax;
  extern const double xPixelMax;
  extern const double yPixelMax;
  QCPColorGradient loadRedToWhiteGradient();
}

#endif // XPEMONPLOTOPTIONS_H
