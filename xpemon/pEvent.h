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
    
    pEvent() : pEventWindow (), m_isEmpty(true) {;}
    
    pEvent(int firstCol, int lastCol, int firstRow, int lastRow,
           const event::Adc_vec_t &adcCounts, microsecond_t microseconds, 
           adc_count_t threshold);
  public:
    
    /* Getters */
    bool isEmpty() const {return m_isEmpty;}
    const std::vector<event::Hit>& hits() const {return m_hits;}
    //access by index number    
    const event::Hit& operator() (int index) const;
    //access by offset coordinates
    const event::Hit& operator() (const OffsetCoordinate& p) const;
    //access by cubic coordinates
    const event::Hit& operator() (const CubeCoordinate& p) const;
    //timestamp of the event
    microsecond_t microseconds() const {return m_microseconds;}
    //index of highest Pixel
    int highestPixelAddress() const 
      {return m_highestPixelAddress;}
    //highest Pixel
    const event::Hit& highestPixel() const
      {return m_hits.at(highestPixelAddress());}
    //highest Pixel row and column
    void highestPixelCoordinates(int& row, int& col) const;
    //total pulse height
    int rawPulseHeight() const
      {return m_rawPulseHeight;}
    //cluster pulse hieght
    int pulseHeight() const
      {return m_pulseHeight;}
    //number of pixels in main cluster
    int clusterSize() const
      {return m_clusterSize;}
    //moment analysis
    const pMomentsAnalysis& moma1() const
      {return m_momentsAnalysis1;}
    const pMomentsAnalysis& moma2() const
      {return m_momentsAnalysis2;}
    double phi() const {return m_momentsAnalysis2.phi();}
    double phiDeg() const {return m_momentsAnalysis2.phiDeg();}
    
    //iterator
    typedef std::vector<event::Hit>::const_iterator const_eventIterator;
    const_eventIterator begin() {return m_hits.begin();}
    const_eventIterator end() {return m_hits.end();}
    
    // Run the event reconstruction.
    void reconstruct(int threshold);

    // Terminal formatting.
    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const pEvent& event)
      {return event.fillStream(os);}
    
    
  private:

    microsecond_t m_microseconds;
    adc_count_t m_threshold; // zero suppression threshold
    int m_highestPixelAddress;  // address of the highest pixel
    int m_rawPulseHeight;  // sum of all pixel counts
    int m_pulseHeight; // sum of all pixel in cluster
    double m_baricenterX;
    double m_baricenterY;
    std::vector<event::Hit> m_hits;  // hits vector
    int m_clusterSize;
    pMomentsAnalysis m_momentsAnalysis1;  // moment analysis info
    pMomentsAnalysis m_momentsAnalysis2;  // moment analysis info
    
    int findHighestPixel() const;
    int minKey(const std::vector<int> &key) const;
    void clusterize(int threshold);
    bool m_isEmpty;
};

#endif //PEVENT_H
