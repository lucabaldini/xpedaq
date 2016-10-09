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

#ifndef PEVENT_H
#define PEVENT_H

#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits.h>
#include <cmath>

#include "pHexagonCoordinates.h"
#include "pEventWindow.h"
#include "pMomentsAnalysis.h"


class pEvent: public pEventWindow
{
  
  public:
    
    pEvent() : pEventWindow (){;}
    
    pEvent(int firstCol, int lastCol,
           int firstRow, int lastRow,
           const event::Adc_vec_t &adcCounts,
           adc_count_t threshold);
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
    //index of highest Pixel
    inline int highestPixelAddress() const 
      {return m_highestPixelAddress;}
    //highest Pixel
    inline const event::Hit& highestPixel() const
      {return m_hits.at(highestPixelAddress());}
    //highest Pixel row and column
    void highestPixelCoordinates(int& row, int& col) const;
    // sum of all pulse heights
    inline adc_count_t totalPulseHeight() const
      {return m_totalPulseHeight;}
    // sum of all pixels in cluster
    inline adc_count_t clusterPulseHeight() const
      {return m_clusterPulseHeight;}
    // x coordinate of the barycenter 
    inline double xBarycenter() const
      {return m_momentsAnalysis.x0();}
    // y coordinate of the barycenter      
    inline double yBarycenter() const
      {return m_momentsAnalysis.y0();}
    // y coordinate of the barycenter   
    inline double phi() const
      {return m_momentsAnalysis.phi();}
    // number of pixels in main cluster   
    inline int clusterSize() const
      {return m_clusterSize;}
    //longitudinal second moment of the cluster
    inline double mom2Long() const
      {return m_momentsAnalysis.mom2long();}
    // transverse second moment of the cluster
    inline double mom2Trans() const 
      {return m_momentsAnalysis.mom2trans();}
    // transverse second moment of the cluster
    inline double skewness() const
      {return m_momentsAnalysis.skewness();}
    
    //iterator
    typedef std::vector<event::Hit>::const_iterator const_eventIterator;
    const_eventIterator begin() {return m_hits.begin();}
    const_eventIterator end() {return m_hits.end();}
    
    void clusterize(int threshold);
    
    int doMomentsAnalysis();

    // Terminal formatting.
    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const pEvent& event)
      {return event.fillStream(os);}
    
    
  protected:
    
    adc_count_t m_threshold; // zero suppression threshold
    int m_highestPixelAddress;  // address of the highest pixel
    adc_count_t m_totalPulseHeight;  // sum of all pixel counts
    adc_count_t m_clusterPulseHeight; // sum of all pixel in cluster
    std::vector<event::Hit> m_hits;  // hits vector
    int m_clusterSize;
    pMomentsAnalysis m_momentsAnalysis;  // moment analysis info

  private:

    int findHighestPixel() const;
    adc_count_t pixelSum(adc_count_t threshold) const;
    int minKey(const std::vector<int> &key) const;
};

#endif //PEVENT_H
