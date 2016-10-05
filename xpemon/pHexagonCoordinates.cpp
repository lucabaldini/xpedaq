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

#include "pHexagonCoordinates.h"

std::vector<CubeCoordinate> CubeCoordinate::neighbours(CubeCoordinate point)
{
  std::vector<CubeCoordinate> neighboursVec;
  neighboursVec.push_back(point + CubeCoordinate(+1, -1,  0));
  neighboursVec.push_back(point + CubeCoordinate(+1,  0, -1));
  neighboursVec.push_back(point + CubeCoordinate( 0, +1, -1));
  neighboursVec.push_back(point + CubeCoordinate(-1, +1,  0));
  neighboursVec.push_back(point + CubeCoordinate(-1,  0, +1));
  neighboursVec.push_back(point + CubeCoordinate( 0, -1, +1));
  return neighboursVec;
}


CubeCoordinate operator+ (const CubeCoordinate &p1,
                          const CubeCoordinate &p2)
{
	// we can access member directly because this is a friend function
	return CubeCoordinate(p1.m_x + p2.m_x, p1.m_y + p2.m_y, p1.m_z + p2.m_z);
}


CubeCoordinate operator- (const CubeCoordinate &p1,
                          const CubeCoordinate &p2)
{
	// we can access member directly because this is a friend function
	return CubeCoordinate(p1.m_x - p2.m_x, p1.m_y - p2.m_y, p1.m_z - p2.m_z);
}



OffsetCoordinate operator+ (const OffsetCoordinate &p1,
                            const OffsetCoordinate &p2)
{
  // we can access member directly because this is a friend function
	return OffsetCoordinate(p1.m_col + p2.m_col, p1.m_row + p2.m_row);
}


OffsetCoordinate operator- (const OffsetCoordinate &p1,
                            const OffsetCoordinate &p2)
{
  // we can access member directly because this is a friend function
	return OffsetCoordinate(p1.m_col - p2.m_col, p1.m_row - p2.m_row);
}


OffsetCoordinate cube2Offset(const CubeCoordinate &p)
{
  int col = p.x() + (p.z() + (p.z()&1)) / 2;
  int row = p.z();
  return OffsetCoordinate(col, row);
}


CubeCoordinate offset2Cube(const OffsetCoordinate &p)
{
  int x = p.col() - (p.row() + (p.row()&1)) / 2;
  int z = p.row();
  int y = -x - z;
  return CubeCoordinate (x,y,z);
}


int distance (const CubeCoordinate &p1, const CubeCoordinate &p2)
{
  return std::max(std::max(std::abs(p1.x() - p2.x()),
                           std::abs(p1.y() - p2.y())),
                  std::abs(p1.z() - p2.z()));
}
