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


pEvent::pEvent(int firstCol, int lastCol, int firstRow, int lastRow,
               const event::Adc_vec_t& adcCounts, int microseconds, 
               adc_count_t threshold):
  pEventWindow(firstCol, lastCol, firstRow, lastRow),
  m_microseconds(microseconds),
  m_threshold(threshold),
  m_clusterSize(0),
  m_isEmpty(false)
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
  m_highestPixelAddress = findHighestPixel();
  m_rawPulseHeight = 0;  
  m_pulseHeight = 0;
  m_baricenterX = 0.;
  m_baricenterY = 0.;
}

int pEvent::findHighestPixel() const
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


void pEvent::highestPixelCoordinates(int& row, int& col) const
{
  OffsetCoordinate p = coordToPixel(highestPixel().x, highestPixel().y);
  row = p.row();
  col = p.col();
}


/* Clustering based on a modified Prim's Minimum Spanning Tree (MST)
   algorithm (http://www.geeksforgeeks.org/greedy-algorithms-set-5-prims-minimum-spanning-tree-mst-2/)
   The MST is built starting from the highest pixel and connecting adjacent
   pixels (excluding those under threshold).
   The MST constructed is the cluster.
*/
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
  for (int i = 0; i < dim - 1; ++i)
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
    m_clusterSize += 1;
    
    /* Update key value and pixel index of the adjacent pixels of
       the picked pixel. Consider only those pixels which are over 
       threshold and not yet included in MST. */
    std::vector<CubeCoordinate> neighbourVec =
                                             cubeCoord(minKeyId).neighbours();
    for (const auto& neighbour : neighbourVec){
      if (!isInWindow(neighbour)) //exclude pixels outside the window
        continue;
      //convert cubic coordinates in array position
      int v = index(neighbour); 
      if (m_hits.at(v).clusterId < 0 && m_hits.at(v).counts > threshold){
        parent[v] = minKeyId;
        key[v] = 1; //the distance between neighbours is always 1
      }
    }
  }
}


/* Utility function for  finding the minimum key between those not yet
   included in the MST.
*/
int pEvent::minKey(const std::vector<int> &key) const
{
   // Initialize min value
   int min = INT_MAX;
   int minIndex = -1;
   for (unsigned int v = 0; v < key.size(); v++){
     if (m_hits.at(v).clusterId < 0 && key.at(v) < min){
       min = key.at(v);
       minIndex = v;
     }
   }
   return minIndex;
}


void pEvent::reconstruct(int threshold)
{
  int clusterId = 0;

  // Run the clustering.
  clusterize(threshold);

  // Calculate the pulse height(s) and the coordinates of the baricenter.
  m_rawPulseHeight = 0;  
  m_pulseHeight = 0;
  m_baricenterX = 0.;
  m_baricenterY = 0.;
  for (const auto &hit : m_hits){
    m_rawPulseHeight += hit.counts;
    if (hit.clusterId == clusterId) {
      m_pulseHeight += hit.counts;
      m_baricenterX += hit.x * hit.counts;
      m_baricenterY += hit.y * hit.counts;
    }
  }
  if (m_pulseHeight > 0) {
    m_baricenterX /= m_pulseHeight;
    m_baricenterY /= m_pulseHeight;
  }

  // Run the first-pass moments analysis.
  m_momentsAnalysis1.run(m_hits, threshold, m_baricenterX, m_baricenterY);

  // Calculate the seed for the conversion point.
  double dmin = 1.5 * m_momentsAnalysis1.rmsLong();
  double dmax = 3.5 * m_momentsAnalysis1.rmsLong();
  double x0 = 0.;
  double y0 = 0.;
  double wsum = 0.;
  double dx, dy, xp, d;
  double cphi = cos(m_momentsAnalysis1.phi());
  double sphi = sin(m_momentsAnalysis1.phi());
  for (const auto &hit : m_hits) {
    if (hit.clusterId == clusterId) {
      dx = (hit.x - m_baricenterX);
      dy = (hit.y - m_baricenterY);
      d = sqrt(pow(dx, 2.) + pow(dy, 2.));
      if (d > dmin && d < dmax) {
	xp = cphi * dx + sphi * dy;
	if (std::signbit(xp) == std::signbit(m_momentsAnalysis1.mom3long())) {
	  x0 += hit.x * hit.counts;
	  y0 += hit.y * hit.counts;
	  wsum += hit.counts;
	}
      }
    }
  }
  if (wsum > 0.) {
    x0 /= wsum;
    y0 /= wsum;
  }

  // Now we can assign a direction to the original axis, based on the
  // sign of the third moment.
  m_momentsAnalysis1.flip3();
  
  // Calculate the weights for the second step of the reconstruction.
  double weightScale = 0.05;
  std::vector<double> weights;
  weights.resize(m_hits.size());
  for (unsigned int index = 0; index < m_hits.size(); ++index) {
    event::Hit hit = m_hits[index];
    if (hit.clusterId == clusterId) {
      d = sqrt(pow((hit.x - x0), 2.) + pow((hit.y - y0), 2.));
      weights[index] = exp(-d/weightScale);
    } else {
      weights[index] = 0.;
    }
  }

  // Finally, run the second-pass moments analysis.
  m_momentsAnalysis2.run(m_hits, threshold, x0, y0, weights);
}


std::ostream& pEvent::fillStream(std::ostream& os) const
{
  os << "w(" << std::setfill(' ') << std::setw(3) << firstCol() << ", "
     << std::setw(3) << lastCol() << ")--(" << std::setw(3) << firstRow()
     << ", " << std::setw(3) << lastRow() << ") " << std::endl;
  return os;
}
