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
  
  struct Hit{
    double x;
    double y;
    adc_count_t counts;
  };
}


class pEvent
{
  
  public:
    
    pEvent(int firstCol, int lastCol,
           int firstRow, int lastRow,
           int bufferId, const event::Adc_vec_t &adcCounts);
  public:
    
    //Getters
    inline const std::vector<event::Hit> &hits() const {return m_hits;}
    inline int buffSize() const {return m_hits.size();}
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
    const event::Hit& operator() (int index) const {return m_hits.at(index);}
    adc_count_t totalAdcCounts() const; // sum of all pulse heights
    int cubeDistance(const OffsetCoordinate &p1,
                     const OffsetCoordinate &p2) const;  
    int highestPixelAddress() const; //index of highest Pixel
    const event::Hit& highestPixel() const; //highest Pixel
    
    // Coordinate transformations
    int index(const OffsetCoordinate &p) const;
    OffsetCoordinate pixelCoord(int index) const;
    int index(const CubeCoordinate &p) const;  
    CubeCoordinate cubeCoord(int index) const;
    void pixelToCoord(const OffsetCoordinate &p, double &x, double &y) const;
    OffsetCoordinate coordToPixel(double x, double y) const;
    

    // Terminal formatting.
    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const pEvent& event)
      {return event.fillStream(os);}
    
    
  protected:

    int m_firstCol;
    int m_lastCol;
    int m_firstRow;
    int m_lastRow;
    int m_bufferId;
    std::vector<event::Hit> m_hits;  // hits
};

#endif //PEVENT_H
