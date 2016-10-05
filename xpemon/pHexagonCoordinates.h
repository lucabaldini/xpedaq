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
    
    inline int x() const {return m_x;}
    inline int y() const {return m_y;}
    inline int z() const {return m_z;}
    
    friend  CubeCoordinate operator+ (const CubeCoordinate &p1,
                                      const CubeCoordinate &p2);
    friend  CubeCoordinate operator- (const CubeCoordinate &p1,
                                      const CubeCoordinate &p2);                                      
    /* Function for finding the coordinates of the neighbours of a hexagon
       in a grid */
    std::vector<CubeCoordinate> neighbours(CubeCoordinate point);

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
    
    inline int row() const {return m_row;}
    inline int col() const {return m_col;}
    
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
int distance (const CubeCoordinate &p1, const CubeCoordinate &p2);

#endif //PHEXAGONCOORDINATES_H
