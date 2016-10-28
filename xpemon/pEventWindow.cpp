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

#include "pEventWindow.h"

// pixel coordinates to order index
int pEventWindow::index(const OffsetCoordinate &p) const
{
  if (isInWindow(p))
    return p.col() - m_firstCol + (p.row() - m_firstRow) * nColumns();
  else return -1;
}

// order index to pixel coordinates
OffsetCoordinate pEventWindow::pixelCoord(int index) const
{
  return OffsetCoordinate(index % nColumns() + m_firstCol,
                          index / nColumns() + m_firstRow);
}

// cube coordinates to order index
int pEventWindow::index(const CubeCoordinate &p) const
{
  if (isInWindow(p))
    return index(cube2Offset(p));
  else return -1;
}

// order index to cube coordinates
CubeCoordinate pEventWindow::cubeCoord(int index) const
{
  return offset2Cube(pixelCoord(index));
}

// offset coordinates to physical xpe coordinates
void pEventWindow::pixelToCoord(const OffsetCoordinate &p,
                                double &x, double &y) const
{
  x = ((p.col()) - 0.5 * (298.5 + (p.row())%2 )) * (xpoldetector::kColPitch);
  y = (175.5 - (p.row())) * (xpoldetector::kRowPitch);
}

// physical xpe coordinates to offset coordinates
OffsetCoordinate pEventWindow::coordToPixel(double x, double y) const
{
  int col, row;  
  row = std::round(175.5 - y/(xpoldetector::kRowPitch));
  col = std::round(x/(xpoldetector::kColPitch) + 0.5 * (298.5 + row%2));
  return OffsetCoordinate(col, row);
}


bool pEventWindow::isInWindow(const OffsetCoordinate& point) const
{
  return (point.row() >= m_firstRow && point.row() <= m_lastRow &&
          point.col() >= m_firstCol && point.col() <= m_lastCol);
}


bool pEventWindow::isInWindow(const CubeCoordinate& point) const
{
  return isInWindow(cube2Offset(point)); 
}
