#ifndef HEXAGONMATRIX_H
#define HEXAGONMATRIX_H

#include "pHexagon.h"
#include <vector>


/* 2D matrix of horizontally contiguos regular hexagons (pointy topped).
   All the dimensions are determined by the distance between the centers
   of two adjacent hexagons in a row
*/

class pHexagonMatrix
{
  public:
    
    pHexagonMatrix(double colPitch) : m_columnPitch(colPitch) {;}
    
    //Draw a matrix nCol*nRow starting from the left uppermost pixel 
    void draw (QCustomPlot *parentPlot, double xStart, double yStart,
               unsigned int nCol, unsigned int nRow, int startOdd = 0);
    void reset();
    
    //getters
    pHexagon* hexagon(unsigned int i);
    double columnPitch(){return m_columnPitch;}
    double rowPitch();
    double hexEdge();
    
  private:

   double m_columnPitch;
   std::vector<pHexagon*> m_hexArray;
};

#endif //HEXAGONMATRIX_H
