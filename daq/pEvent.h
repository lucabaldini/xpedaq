#ifndef PEVENT_H
#define PEVENT_H

#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "pDataBlock.h"
#include "pHexagonCoordinates.h"

namespace event
{
  typedef std::vector<adc_count_t> Adc_vec_t;
  const double colPitch = 0.0500; // [mm]
  const double rowPitch = 0.0433; //[mm]
}

class pEvent
{
  
  public:
    
    pEvent(int firstCol, int lastCol,
           int firstRow, int lastRow,
           int bufferId, event::Adc_vec_t adcCounts);
  public:
    
    //Getters
    inline const event::Adc_vec_t & adcCounts() const {return m_adcCounts;}
    inline int buffSize() const {return m_adcCounts.size();}
    inline int firstCol() const {return m_firstCol;}
    inline int lastCol() const {return m_lastCol;}
    inline int firstRow() const {return m_firstRow;}
    inline int lastRow() const {return m_lastRow;}
    inline int bufferId() const {return m_bufferId;}
    inline int nColumns() const {return m_lastCol - m_firstCol + 1;}
    inline int nRows() const {return m_lastRow - m_firstRow + 1;}
    inline int evtSize() const {return nRows() * nColumns();}
    adc_count_t pixelCounts(const OffsetCoordinate &p) const;
    adc_count_t pixelCounts(const CubeCoordinate &p) const;
    const adc_count_t& operator()(int index) const
    {return m_adcCounts.at(index);}
      adc_count_t totalAdcCounts() const; // sum of all pulse heights
    
    int cubeDistance(const OffsetCoordinate &p1,
                     const OffsetCoordinate &p2) const;  
    int highestPixel() const;
    
    // Terminal formatting.
    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const pEvent& event)
    {return event.fillStream(os);}
    
    
  protected:
  
    // Coordinate transformations
    int index(const OffsetCoordinate &p) const;
    OffsetCoordinate pixelCoord(int index) const;
    int index(const CubeCoordinate &p) const;  
    CubeCoordinate cubeCoord(int index) const;
    void pixelToCoord(int i, int j, double &x, double &y) const;
    void coordToPixel(double x, double y, int &i, int &j) const;
    
    int m_firstCol;
    int m_lastCol;
    int m_firstRow;
    int m_lastRow;
    int m_bufferId;
    event::Adc_vec_t m_adcCounts;  //pulse heights
    
};

#endif //PEVENT_H
