/***********************************************************************
Copyright (C) 2007--2016 the X-ray Polarimetry Explorer (XPE) team.

For the license terms see the file LICENSE, distributed along with this
software.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
***********************************************************************/

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
const unsigned int xpemonPlotOptions::modulationNbins = 180;
const double xpemonPlotOptions::modulationThetaMin = 0.;
const double xpemonPlotOptions::modulationThetaMax = 6.19;

QCPColorGradient xpemonPlotOptions::loadRedToWhiteGradient(){
  QCPColorGradient gradient = QCPColorGradient();
  gradient.setColorInterpolation(QCPColorGradient::ciRGB);
  gradient.setColorStopAt(0, QColor(50, 0, 0));
  gradient.setColorStopAt(0.2, QColor(180, 0, 0));
  gradient.setColorStopAt(0.4, QColor(240, 30, 00));
  gradient.setColorStopAt(0.6, QColor(255, 110, 60));
  gradient.setColorStopAt(0.8, QColor(255, 150, 100));
  gradient.setColorStopAt(1, QColor(255, 255, 250));
  return gradient;
}
