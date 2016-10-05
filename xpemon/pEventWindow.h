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

#ifndef PEVENTWINDOW_H
#define PEVENTWINDOW_H

#include "xpoldetector.h"
#include "pHexagonCoordinates.h"

/* Class describing a list of pixels forming a window in the
   detector. Pixel are indicized in the same order as they are read from
   the daq buffer (i.e. row by row in increasing column number) */
   
class pEventWindow{

  public:
  
    pEventWindow(): m_firstCol(0), m_lastCol(0),
                    m_firstRow(0), m_lastRow(0)
                    {;}
    pEventWindow(int firstCol, int lastCol, int firstRow, int lastRow) : 
                 m_firstCol(firstCol), m_lastCol(lastCol),
                 m_firstRow(firstRow), m_lastRow(lastRow)
                 {;}
      
    //getters
    inline int firstCol() const {return m_firstCol;}
    inline int lastCol() const {return m_lastCol;}
    inline int firstRow() const {return m_firstRow;}
    inline int lastRow() const {return m_lastRow;}
    inline int nColumns() const {return m_lastCol - m_firstCol + 1;}
    inline int nRows() const {return m_lastRow - m_firstRow + 1;}
    inline int evtSize() const {return nRows() * nColumns();}    
    
    // Coordinate transformations
    void pixelToCoord(const OffsetCoordinate &p, double &x, double &y) const;
    OffsetCoordinate coordToPixel(double x, double y) const; 
    int index(const OffsetCoordinate &p) const;
    OffsetCoordinate pixelCoord(int index) const;
    int index(const CubeCoordinate &p) const;  
    CubeCoordinate cubeCoord(int index) const;
    
    // Distance in cubic coordinates
    int cubeDistance(const OffsetCoordinate &p1,
                     const OffsetCoordinate &p2) const;    
    
  protected:
  
    int m_firstCol;
    int m_lastCol;
    int m_firstRow;
    int m_lastRow;

};

#endif //PEVENTWINDOW_H
