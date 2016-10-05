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

#include "pEvent.h"

pEvent::pEvent(int firstCol, int lastCol,
               int firstRow, int lastRow,
               const event::Adc_vec_t& adcCounts):
               pEventWindow(firstCol, lastCol, firstRow, lastRow)
{
  if (adcCounts.size() != nRows() * nColumns()) {
    std::cout << "WARNING: Buffer does not fit window size passed"
              << std::endl;
  }
  for (unsigned int i =0; i < adcCounts.size(); ++i){
    double x, y;
    pixelToCoord(pixelCoord(i), x, y);
    m_hits.push_back(event::Hit{x, y, adcCounts.at(i)});
  }
  m_cluster = pCluster();
}


adc_count_t pEvent::totalAdcCounts() const
{
  using namespace event;
  adc_count_t sum = 0; 
  for(std::vector<event::Hit>::const_iterator it = m_hits.begin();
      it != m_hits.end();
      ++it)
   {
     sum += (*it).counts;
   }
   return sum;
}


int pEvent::highestPixelAddress() const
{
  int maxPos = -1;
  int maxVal = 0;
  for (int i =0; i < evtSize(); ++i){
    if (m_hits.at(i).counts > maxVal){
      maxVal = m_hits.at(i).counts;
      maxPos = i;
    }
  }
  return maxPos;
}


const event::Hit& pEvent::highestPixel() const
{
  return m_hits.at(highestPixelAddress());
}


void pEvent::clusterize(int threshold)
{
  m_cluster.build(*this, hits(), threshold);
}


std::ostream& pEvent::fillStream(std::ostream& os) const
{
  os << "w(" << std::setfill(' ') << std::setw(3) << firstCol() << ", "
     << std::setw(3) << lastCol() << ")--(" << std::setw(3) << firstRow()
     << ", " << std::setw(3) << lastRow() << ") " << std::endl;
  return os;
}
