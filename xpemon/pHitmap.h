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

#ifndef PHITMAP_H
#define PHITMAP_H

#include "pMapPlot.h"
#include "pHistogramOptions.h"

class pHitmap : public pMapPlot
{
  
  Q_OBJECT
  
  public:
  
    pHitmap(const pMap* map, pColorMapOptions options = pColorMapOptions());
    
  protected slots:

    void xAxis2Update(QCPRange range);
    void yAxis2Update(QCPRange range);
    virtual void paintCoordinate();
    virtual void resizeEvent (QResizeEvent* event);
    void setXaxisTickStep(QCPRange range);
    void setYaxisTickStep(QCPRange range);
  
  protected:
  
    void setupAxesConnections();
    void offsetToPhysical(int col, int row, double &x, double &y) const;
    void phyisicalToOffset(double x, double y, int &col, int &row) const;
    void adjustExternalMarginsForSize(int size);
    void forceSquaredAspectRatio();
    const int m_minPlotEdge; //minimum size of the map edge (in pixels)
};

#endif  //PHITMAP_H

