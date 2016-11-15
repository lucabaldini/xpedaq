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

#include "pHitmap.h"
#include "xpoldetector.h"

pHitmap::pHitmap(const pMap* map, pColorMapOptions options) :
  pMapPlot(map, options),
  m_minPlotEdge(400)
{
  //Initialize coordinate axes with default range
  xAxis2->setRange(-7.4875, 7.4875);
  xAxis2->setLabel("x[mm]");
  xAxis2->setVisible(true);
  xAxis2->setTickLabels(true);
  yAxis2->setRange(-7.5991, 7.599);
  yAxis2->setLabel("y[mm]");
  yAxis2->setVisible(true);
  yAxis2->setTickLabels(true);
  synchronizeAxes();
}

void pHitmap::adjustExternalMarginsForSize(int size)
{
  return;
}


void pHitmap::forceSquaredAspectRatio()
{
  return;
}


void pHitmap::pixelToCoord(int col, int row, double &x, double &y) const
{
  x = (col - 0.5 * (298.5 + row%2 )) * (xpoldetector::kColPitch);
  y = (175.5 - row) * (xpoldetector::kRowPitch);
}


void pHitmap::coordToPixel(double x, double y, int &col, int &row) const
{
  row = std::round(175.5 - y/(xpoldetector::kRowPitch));
  col = std::round(x/(xpoldetector::kColPitch) + 0.5 * (298.5 + row%2));
}


void pHitmap::xAxis2Update(QCPRange range)
{
  double xMin, xMax, y;
  pixelToCoord(range.lower, 0., xMin, y);
  pixelToCoord(range.upper, 0., xMax, y);
  xAxis2->setRange(xMin, xMax);
}


void pHitmap::yAxis2Update(QCPRange range)
{
  double yMin, yMax, x;
  pixelToCoord(0., range.lower, x, yMin);
  pixelToCoord(0., range.upper, x, yMax);
  yAxis2->setRange(yMin, yMax);
}


void pHitmap::synchronizeAxes()
{
  // Keep axes synchronized
  connect(xAxis, SIGNAL(rangeChanged(QCPRange)),
          this, SLOT(xAxis2Update(QCPRange)));
  connect(yAxis, SIGNAL(rangeChanged(QCPRange)),
          this, SLOT(yAxis2Update(QCPRange)));
}
