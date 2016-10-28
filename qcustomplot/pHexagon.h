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

#ifndef PHEXAGON_H
#define PHEXAGON_H

#include <cmath>
#include <QVector>

#include "qcustomplot.h"

/* Class describing a hexagon */

class pHexagon : public QCPCurve
{

  public:
  
    //Draw a regular hexagon at position (posX, posY) with edge 'size'
    pHexagon(double posX, double posY, double size, QCustomPlot *parentPlot);
    
    //Check if the mouse is inside the hexagon
    //virtual double selectTest (const QPointF &pos, bool onlySelectable,
    //                           QVariant* details=0) const;

};

#endif //PHEXAGON_H
