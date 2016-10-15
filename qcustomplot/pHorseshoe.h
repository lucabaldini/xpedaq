/***********************************************************************
Copyright (C) 2016 the X-ray Polarimetry Explorer (XPE) team.

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


#ifndef PHORSESHOE_H
#define PHORSESHOE_H

#include "qcustomplot.h"


class pHorseshoe : public QCPCurve
{
  public:
    
    pHorseshoe(QCustomPlot* parent, const QColor &color, int lineWidth,
	       const Qt::PenStyle linStyle, int numPoints = 100);
    void setup(double x0, double y0, double rmin, double rmax, double phi0);
    void clear();

    //virtual void draw(QCPPainter *painter);

  private:
    
    int m_numPoints;
};

#endif //PHORSESHOE_H
