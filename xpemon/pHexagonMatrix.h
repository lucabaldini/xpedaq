#ifndef HEXAGONMATRIX_H
#define HEXAGONMATRIX_H

#include "pHexagon.h"
#include "qcustomplot.h"
#include <vector>


/* 2D matrix of horizontally contiguos regular hexagons (pointy topped).
   All the dimensions are determined by the distance between the centers
   of two adjacent hexagons in a row
*/

class pHexagonMatrix
{
  public:
    
    pHexagonMatrix(double colPitch) : m_columnPitch(colPitch),
                                      m_xStart(0.),
                                      m_yStart(0.),
                                      m_nCol(0),
                                      m_nRow(0),
                                      m_firstLeft(false),
                                      m_padding(0.) {;}
    
    //Draw a matrix nCol*nRow starting from the left uppermost pixel 
    void draw (QCustomPlot *parentPlot, double xStart, double yStart,
               int nCol, int nRow, bool firstLeft = false,
               double padding = 0.);
    void reset();
    
    //getters
    pHexagon* hexagon(int i);
    double columnPitch(){return m_columnPitch;}
    double rowPitch();
    double hexEdge();
    int nRow(){return m_nRow;}
    int nColumn(){return m_nCol;}
    
    //iterator
    typedef std::vector<pHexagon*>::iterator matrixIterator;
    typedef std::vector<pHexagon*>::const_iterator const_matrixIterator;
    matrixIterator begin() {return m_hexArray.begin();}
    matrixIterator end() {return m_hexArray.end();}
    
  private:

   double m_columnPitch;
   double m_xStart;
   double m_yStart;
   int m_nCol;
   int m_nRow;
   bool m_firstLeft; // first row is left shifted (odd rows in xpe)
   std::vector<pHexagon*> m_hexArray;
   double m_padding;
};

#endif //HEXAGONMATRIX_H
