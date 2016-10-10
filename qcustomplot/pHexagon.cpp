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

#include "pHexagon.h"

//see http://www.redblobgames.com/grids/hexagons/

pHexagon::pHexagon(double posX, double posY, double size,
                   QCustomPlot *parentPlot):
                   QCPCurve(parentPlot->xAxis, parentPlot->yAxis)
{
  const int nPoints = 7; // we need an extra point to close the curve
  QVector<double> vertexIndex(nPoints);
  QVector<double> xCoord(nPoints);
  QVector<double> yCoord(nPoints);
  for (int i =0; i < nPoints; ++i){
      double angleDeg = 60. * i   + 30.;
      double angleRad = (PI / 180.) * angleDeg;
      vertexIndex[i] = i;
      xCoord[i] = posX + size * cos(angleRad);
      yCoord[i] = posY + size * sin(angleRad);
  }
  setData (vertexIndex, xCoord, yCoord);
  setLineStyle(QCPCurve::lsLine);
  setPen(QPen(Qt::black));
  setScatterStyle(QCPScatterStyle::ssDot);
  parentPlot->addPlottable(this);
}
