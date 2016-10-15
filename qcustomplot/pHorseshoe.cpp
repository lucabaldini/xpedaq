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


#include "pHorseshoe.h"


pHorseshoe::pHorseshoe(QCustomPlot* parent, const QColor &color, int lineWidth,
		       const Qt::PenStyle lineStyle, int numPoints) :
  QCPCurve(parent->xAxis, parent->yAxis),
  m_numPoints(numPoints)
{
  // Setup the underlying QPen object.
  QPen pen = QPen();
  pen.setWidth(lineWidth);
  pen.setColor(color);
  pen.setStyle(lineStyle);
  setPen(pen);
}
  

void pHorseshoe::setup(double x0, double y0, double rmin, double rmax,
		       double phi0)
{
  QVector<double> x, y;
  double phi;
  // Min we have to one arc clockwise...
  for (int i = 0; i < m_numPoints; ++i)
    {
      phi = phi0 + (m_numPoints - i - 1)/(double)(m_numPoints - 1)*M_PI;
      x.push_back(x0 - rmin*cos(phi));
      y.push_back(y0 - rmin*sin(phi));
    }
  // ...and the other anti-clockwise not to have an extra segment.
  for (int i = 0; i < m_numPoints; ++i)
    {
      phi = phi0 + i/(double)(m_numPoints - 1)*M_PI;
      x.push_back(x0 - rmax*cos(phi));
      y.push_back(y0 - rmax*sin(phi));
    }
  x.push_back(x.at(0));
  y.push_back(y.at(0));
  setData(x, y);
}


void pHorseshoe::clear()
{
  QVector<double> x, y;
  setData(x, y);
}
