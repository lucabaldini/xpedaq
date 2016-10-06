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
    m_hits.push_back(event::Hit{x, y, adcCounts.at(i), -1});
  }
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


/* Utility function for  finding the minimum key between those not yet
   included in the MST.
*/
int pEvent::minKey(const std::vector<int> &key) const
{
   // Initialize min value
   int min = INT_MAX;
   int minIndex = -1;
   for (int v = 0; v < key.size(); v++){
     if (m_hits.at(v).clusterId < 0 && key.at(v) < min){
       min = key.at(v);
       minIndex = v;
     }
   }
   return minIndex;
}


void pEvent::clusterize(int threshold)
{
  int dim = evtSize();
  std::vector<int> parent; // Array to store constructed MST
  std::vector<int> key;   // Key values used to pick minimum weight edge
  
  // Initialize all keys as INT_MAX
  for (int i = 0; i < dim; ++i){
    key.push_back(INT_MAX);
    parent.push_back(-1); // non physical init
  }
  
  // Start from higest pixel in MST.
  // Make key 0 so that this pixel is picked as first pixel
   int highestAddress = highestPixelAddress();
  key[highestAddress] = 0;
  parent[highestAddress] = highestAddress; //This pixel is root of MST

  // Loop on all the pixels
  for (int index = 0; index < dim - 1; ++index)
  {
    /* Pick the minimum key pixel from the set of pixels not yet included
       in MST */
    int minKeyId = minKey(key); //return -1 if no pixel is found
    /* A result -1 means all pixels not included in MST are under theshold
       or not adjacent to any included pixel. In that case the cluster is
       complete and we exit the loop. */
    if (minKeyId < 0)
      break;
    // Else add the picked pixel to the MST Set
    m_hits[minKeyId].clusterId = 0;
    
    /* Update key value and pixel index of the adjacent pixels of
       the picked pixel. Consider only those pixels which are not yet
       included in MST. */
    for (int v = 0; v < dim; ++v){
      if (m_hits.at(v).clusterId < 0){ //check only pixels not yet included in MST
        /* Update the key only if distance from minKeyId is smaller than
           key[v] and is less than 2 (adjacent pixels).
           Ignore under threshold pixels. */
        if (m_hits.at(v).counts >= threshold){
          int dist = cubeDistance(pixelCoord(minKeyId), pixelCoord(v));
          if (dist && dist < 2 && dist <  key[v]){
            parent[v] = minKeyId;
            key[v] = dist;
          }
        }
      }
    }
  }
}


int pEvent::doMomentsAnalysis()
{
 /*
 if pivot is None:
            pivot = cluster.baricenter
        self.pivot = pivot
        w = cluster.adc_values*weights
        wsum = numpy.sum(w)
        # Calculate the offsets with respect to the pivot.
        dx = (cluster.x - pivot.x())
        dy = (cluster.y - pivot.y())
        # Solve for the angle of the principal axis (note that at this point
        # phi is comprised between -pi/2 and pi/2 and might indicate either
        # the major or the minor axis of the tensor of inertia).
        A = numpy.sum(dx*dy*w)
        B = numpy.sum((dy**2. - dx**2.)*w)
        phi = -0.5*numpy.arctan2(2.*A, B)
        # Rotate by an angle phi and calculate the eigenvalues of the tensor
        # of inertia.
        xp = numpy.cos(phi)*dx + numpy.sin(phi)*dy
        yp = -numpy.sin(phi)*dx + numpy.cos(phi)*dy
        mom2_long = numpy.sum((xp**2.)*w)/wsum
        mom2_trans = numpy.sum((yp**2.)*w)/wsum
        # We want mom2_long to be the highest eigenvalue, so we need to
        # check wheteher we have to swap the eigenvalues, here. Note that
        # at this point phi is still comprised between -pi/2 and pi/2.
        if mom2_long < mom2_trans:
            mom2_long, mom2_trans = mom2_trans, mom2_long
            phi -= 0.5*numpy.pi*numpy.sign(phi)
        # Set the class members.
        self.phi = phi
        self.mom2_long = mom2_long
        self.mom2_trans = mom2_trans
   */
  // Calculate the barycenter of the cluster a
  double x0 = 0.;
  double y0 = 0.;
  double pulseHeight = 0.;
  for (const event::Hit& hit : m_hits) {
    x0 += hit.x * hit.counts;
    y0 += hit.y * hit.counts;
    pulseHeight += hit.counts;
  }
  x0 /= pulseHeight;
  y0 /= pulseHeight;
  //std::cout << x0 << " " << y0  << std::endl;
}


std::ostream& pEvent::fillStream(std::ostream& os) const
{
  os << "w(" << std::setfill(' ') << std::setw(3) << firstCol() << ", "
     << std::setw(3) << lastCol() << ")--(" << std::setw(3) << firstRow()
     << ", " << std::setw(3) << lastRow() << ") " << std::endl;
  return os;
}
