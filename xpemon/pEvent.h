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

#include "pHexagonCoordinates.h"
#include "pCluster.h"
#include "pEventWindow.h"

class pEvent: public pEventWindow
{
  
  public:
    
    pEvent() : pEventWindow (){;}
    
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
    
    //iterator
    typedef std::vector<event::Hit>::const_iterator const_eventIterator;
    const_eventIterator begin() {return m_hits.begin();}
    const_eventIterator end() {return m_hits.end();}
    
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
