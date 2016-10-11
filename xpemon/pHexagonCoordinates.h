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

#ifndef PHEXAGONCOORDINATES_H
#define PHEXAGONCOORDINATES_H

#include <cmath>
#include <vector>


/* Class describing a cubic coordinate system for an hexagonal grid.
   See also //see http://www.redblobgames.com/grids/hexagons/ */
class CubeCoordinate
{
  public:
     
    CubeCoordinate() : m_x(0), m_y(0), m_z(0) {;}
    CubeCoordinate(int x, int y, int z) : m_x(x), m_y(y), m_z(z) {;}
    
    int x() const {return m_x;}
    int y() const {return m_y;}
    int z() const {return m_z;}
    
    friend  CubeCoordinate operator+ (const CubeCoordinate &p1,
                                      const CubeCoordinate &p2);
    friend  CubeCoordinate operator- (const CubeCoordinate &p1,
                                      const CubeCoordinate &p2);                                      
    /* Function for finding the coordinates of the neighbours of a hexagon
       in a grid */
    std::vector<CubeCoordinate> neighbours();

 private:    
   
    int m_x;
    int m_y;
    int m_z;
};


/* Class describing (column, row) coordinate pairs in a grid */
class OffsetCoordinate
{
  public:
     
    OffsetCoordinate() : m_col(0), m_row(0) {;}
    OffsetCoordinate(int col, int row) : m_col(col), m_row(row) {;}
    
    int row() const {return m_row;}
    int col() const {return m_col;}
    
    friend  OffsetCoordinate operator+ (const OffsetCoordinate &p1,
                                        const OffsetCoordinate &p2);
    friend  OffsetCoordinate operator- (const OffsetCoordinate &p1,
                                        const OffsetCoordinate &p2);                                        

 private:    
   
    int m_col;
    int m_row;    
};


/* Covertion functions for the even-r case (even rows are right shifted) */
OffsetCoordinate cube2Offset(const CubeCoordinate &p);
CubeCoordinate offset2Cube(const OffsetCoordinate &p);


/* Distance in cubic coordinate */
int cubicDistance (const CubeCoordinate &p1, const CubeCoordinate &p2);

#endif //PHEXAGONCOORDINATES_H
