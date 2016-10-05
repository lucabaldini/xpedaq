#ifndef PEVENTWINDOW_H
#define PEVENTWINDOW_H

#include "xpoldetector.h"
#include "pHexagonCoordinates.h"

/* Class describing a list of pixels forming a window in the
   detector. Pixel are indicized in the same order as they are read from
   the daq buffer (i.e. row by row in increasing column number) */
   
class pEventWindow{

  public:
  
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
