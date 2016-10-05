#include <limits.h>
#include <iostream>
#include <algorithm>

#include "pCluster.h"

/* Clustering based on a modified Prim's Minimum Spanning Tree (MST)
   algorithm (http://www.geeksforgeeks.org/greedy-algorithms-set-5-prims-minimum-spanning-tree-mst-2/)
   The MST is built starting from the highest pixel and connecting adjacent
   pixels (excluding those under threshold).
   The MST constructed is the cluster.
*/

static bool compare(const event::Hit& a, const event::Hit& b)
{
    return (a.counts < b.counts);
}


void pCluster::build(const pEventWindow &evt,
                     const std::vector<event::Hit> &hits,
                     int threshold)
{
  int dim = evt.evtSize();
  std::vector<int> parent; // Array to store constructed MST
  std::vector<int> key;   // Key values used to pick minimum weight edge
  
  // Initialize all keys as INT_MAX
  for (int i = 0; i < dim; ++i){
    key.push_back(INT_MAX);
    m_isInCluster.push_back(false);
    parent.push_back(-1); // non physical init
  }
  
  // Start from higest pixel in MST.
  // Make key 0 so that this pixel is picked as first pixel
  std::vector<event::Hit>::const_iterator maxFinder;
  maxFinder = std::max_element(hits.begin(), hits.end(), compare);
  int highestAddress = std::distance(hits.begin(), maxFinder);
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
    m_isInCluster[minKeyId] = true;
 
    /* Update key value and pixel index of the adjacent pixels of
       the picked pixel. Consider only those pixels which are not yet
       included in MST. */
    for (int v = 0; v < dim; ++v){
      if (!m_isInCluster[v]){ //check only pixels not yet included in MST
        /* Update the key only if distance from minKeyId is smaller than
           key[v] and is less than 2 (adjacent pixels).
           Ignore under threshold pixels. */
        if (hits.at(v).counts >= threshold){
          int dist = evt.cubeDistance(evt.pixelCoord(minKeyId),
                                      evt.pixelCoord(v));
          if (dist && dist < 2 && dist <  key[v]){
            parent[v] = minKeyId;
            key[v] = dist;    
          }
        }
      }
    }
  }
  /* The address of pixels where parent is > 0 are the pixel of the cluster */
  
  /*** DEBUG ***/
  //for (int row=0; row<evt.nRows(); ++row){
  //  for (int col=0; col<evt.nColumns(); ++col){
  //    int index = row*evt.nColumns() + col;
  //    if (m_isInCluster.at(index))
  //      std::cout << counts.at(index) << " ";
  //    else
  //      std::cout << "// ";
  //  }
  //  std::cout << std::endl;
  //}
  /******/
}


/* Utility function for  finding the minimum key between those not yet
   included in the MST */
int pCluster::minKey(const std::vector<int> &key) const
{
   // Initialize min value
   int min = INT_MAX;
   int min_index = -1;
 
   for (int v = 0; v < key.size(); v++){
     if (m_isInCluster.at(v) == false && key.at(v) < min){
       min = key.at(v);
       min_index = v;
     }
   }
   return min_index;
}


std::ostream& pCluster::fillStream(std::ostream& os) const
{
  for (auto inCluster : m_isInCluster) // access by const reference
    os << inCluster << " ";
  os << std::endl;
  return os;
}
