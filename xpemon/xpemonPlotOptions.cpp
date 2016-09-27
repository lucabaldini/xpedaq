#include "xpemonPlotOptions.h"

const unsigned int xpemonPlotOptions::pulseHeightNbins = 100;
const double xpemonPlotOptions::pulseHeightXmin = 0.;
const double xpemonPlotOptions::pulseHeightXmax = 10000.;
const unsigned int xpemonPlotOptions::windowSizeNbins = 100;
const double xpemonPlotOptions::windowSizeXmin = 0.;
const double xpemonPlotOptions::windowSizeXmax = 2000.;
const double xpemonPlotOptions::xPixelMax =
                              static_cast<double> (xpoldetector::kNumPixelsX);
const double xpemonPlotOptions::yPixelMax =
                              static_cast<double> (xpoldetector::kNumPixelsY);

QCPColorGradient xpemonPlotOptions::loadRedToWhiteGradient(){
  QCPColorGradient gradient = QCPColorGradient();
  gradient.setColorInterpolation(QCPColorGradient::ciRGB);
  gradient.setColorStopAt(0, QColor(50, 0, 0));
  gradient.setColorStopAt(0.2, QColor(180, 0, 0));
  gradient.setColorStopAt(0.4, QColor(240, 30, 00));
  gradient.setColorStopAt(0.6, QColor(255, 110, 80));
  gradient.setColorStopAt(0.8, QColor(255, 180, 150));
  gradient.setColorStopAt(1, QColor(255, 255, 255));
  return gradient;
}
