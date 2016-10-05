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
  extern const unsigned int modulationNbins;  
  extern const double modulationThetaMin;
  extern const double modulationThetaMax;
  extern const double xPixelMax;
  extern const double yPixelMax;
  QCPColorGradient loadRedToWhiteGradient();
}

#endif // XPEMONPLOTOPTIONS_H
