#ifndef PEVENT_H
#define PEVENT_H

#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "pHexagonCoordinates.h"
#include "pCluster.h"
#include "pEventWindow.h"

class pEvent: public pEventWindow
{
  
  public:
    
    pEvent(int firstCol, int lastCol,
           int firstRow, int lastRow,
           const event::Adc_vec_t &adcCounts);
  public:
    
    /* Getters */
    inline const std::vector<event::Hit>& hits() const {return m_hits;}
    //access by index number    
    const event::Hit& operator() (int index) const {return m_hits.at(index);}
    //access by offset coordinates
    const event::Hit& operator() (const OffsetCoordinate& p) const
      {return m_hits.at(index(p));}
    //access by cubic coordinates
    const event::Hit& operator() (const CubeCoordinate& p) const
      {return m_hits.at(index(p));} 
    adc_count_t totalAdcCounts() const; // sum of all pulse heights
    int highestPixelAddress() const; //index of highest Pixel
    const event::Hit& highestPixel() const; //highest Pixel
    
    void clusterize(int threshold);

    // Terminal formatting.
    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const pEvent& event)
      {return event.fillStream(os);}
    
    
  protected:
    
    std::vector<event::Hit> m_hits;  // hits
    pCluster m_cluster;
       
};

#endif //PEVENT_H
