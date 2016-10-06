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
               const event::Adc_vec_t& adcCounts, 
               adc_count_t threshold):
               pEventWindow(firstCol, lastCol, firstRow, lastRow),
               m_threshold(threshold)
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
  m_pixelHeight = pixelSum(m_threshold);
}


adc_count_t pEvent::pixelSum(adc_count_t threshold) const
{
  adc_count_t sum = 0; 
  for (const event::Hit& hit : m_hits){
    if (hit.counts >= threshold)
      sum += hit.counts;
  }
  return sum;
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


int pEvent::doMomentsAnalysis()
{
  // Calculate the barycenter of the cluster a
  adc_count_t threshold = m_threshold;
  double x0 = 0.;
  double y0 = 0.;
  double pulseHeight = 0.;
  double weight = 1.;
  std::vector<double> w;
  double wsum = 0.;
  for (const event::Hit& hit : m_hits) {
    if (hit.counts >= threshold && hit.clusterId == 0){
      x0 += hit.x * hit.counts;
      y0 += hit.y * hit.counts;
      pulseHeight += hit.counts;
      w.push_back(hit.counts * weight);
      wsum += hit.counts * weight;
    }
  }
  x0 /= pulseHeight;
  y0 /= pulseHeight;
  // Calculate the offsets with respect to the barycenter.
  std::vector<double> dx;
  std::vector<double> dy;
  for (const event::Hit& hit : m_hits){
    if (hit.counts >= threshold && hit.clusterId == 0){
      dx.push_back(hit.x - x0);
      dy.push_back(hit.y - y0);
    }
  }
  // Solve for the angle of the principal axis (note that at this point
  // phi is comprised between -pi/2 and pi/2 and might indicate either
  // the major or the minor axis of the tensor of inertia).
  double A = 0;
  double B = 0;
  for (unsigned int i = 0; i < dx.size(); ++i){
    A += (dx.at(i) * dy.at(i) * w.at(i));
    B += ((pow(dy.at(i), 2.) - pow(dx.at(i), 2.)) * w.at(i));
  }
  double phi = -0.5 * atan2(2*A, B);
  // Rotate by an angle phi and calculate the eigenvalues of the tensor
  // of inertia.
  std::vector<double> xp;
  std::vector<double> yp;
  for (unsigned int i = 0; i < dx.size(); ++i){
    xp.push_back(cos(phi) * dx.at(i) + sin(phi) * dy.at(i));
    yp.push_back(sin(phi) * dx.at(i) + cos(phi) * dy.at(i));
  }
  double mom2long = 0.;
  double mom2trans = 0.;
  for (unsigned int i = 0; i < xp.size(); ++i){
    mom2long += (pow(xp.at(i), 2.) * w.at(i));
    mom2trans += (pow(yp.at(i), 2.) * w.at(i));
  }
  mom2long /= wsum;
  mom2trans /= wsum;
  // We want mom2long to be the highest eigenvalue, so we need to
  // check wheteher we have to swap the eigenvalues, here. Note that
  // at this point phi is still comprised between -pi/2 and pi/2.
  if (mom2long < mom2trans){
    double tmp = mom2long;
    mom2long = mom2trans;
    mom2trans = tmp;
    phi -= 0.5 * M_PI * (2 * (phi > 0) -1);
  }
  // Set the class members.
  m_momentsAnalysis.setX0(x0);
  m_momentsAnalysis.setY0(y0);
  m_momentsAnalysis.setPhi(phi);
  m_momentsAnalysis.setMom2long(mom2long);
  m_momentsAnalysis.setMom2trans(mom2trans);
  //std::cout << x0 << " " << y0 << " " << phi << " " << mom2long << " " 
  //          << mom2trans << std::endl;
  return 0;
}


std::ostream& pEvent::fillStream(std::ostream& os) const
{
  os << "w(" << std::setfill(' ') << std::setw(3) << firstCol() << ", "
     << std::setw(3) << lastCol() << ")--(" << std::setw(3) << firstRow()
     << ", " << std::setw(3) << lastRow() << ") " << std::endl;
  return os;
}
