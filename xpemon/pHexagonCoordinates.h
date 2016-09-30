#ifndef PHEXAGONCOORDINATES_H
#define PHEXAGONCOORDINATES_H

#include <cmath>
#include <vector>

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
    std::vector<CubeCoordinate> neighbours(CubeCoordinate point);

 private:    
   
    int m_x;
    int m_y;
    int m_z;
};

class OffsetCoordinate  // even-r case
{
  public:
     
    OffsetCoordinate() : m_col(0), m_row(0) {;}
    OffsetCoordinate(int col, int row) : m_col(col), m_row(row) {;}
    
    inline int row() const {return m_row;}
    inline int col() const {return m_col;}
    
    friend  OffsetCoordinate operator+ (const OffsetCoordinate &p1,
                                        const OffsetCoordinate &p2);

 private:    
   
    int m_col;
    int m_row;    
};

OffsetCoordinate cube2Offset(const CubeCoordinate &p);
CubeCoordinate offset2Cube(const OffsetCoordinate &p);
int distance (const CubeCoordinate &p1, const CubeCoordinate &p2);

#endif //PHEXAGONCOORDINATES_H
