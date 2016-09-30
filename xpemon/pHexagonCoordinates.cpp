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


OffsetCoordinate operator+ (const OffsetCoordinate &p1,
                            const OffsetCoordinate &p2)
{
  // we can access member directly because this is a friend function
	return OffsetCoordinate(p1.m_col + p2.m_col, p1.m_row + p2.m_row);
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
