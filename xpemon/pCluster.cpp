#include "pCluster.h"

/* Clustering based on a modified Prim's Minimum Spanning Tree (MST)
   algorithm (http://www.geeksforgeeks.org/greedy-algorithms-set-5-prims-minimum-spanning-tree-mst-2/)
   The MST is built starting from the highest pixel and connecting adjacent
   pixels (excluding those under threshold).
*/
pCluster::pCluster(const pEvent &evt, int threshold)
{
  int dim = evt.evtSize();
  std::vector<int> parent; // Array to store constructed MST
  std::vector<int> key;   // Key values used to pick minimum weight edge
  std::vector<bool>  mstSet;  // To represent set of pixels included in MST
  
  // Initialize all keys as INT_MAX
  for (int i = 0; i < dim; ++i){
    key.push_back(INT_MAX);
    mstSet.push_back(false);
    parent.push_back(-1); // non physical init
  }
  
  // Start from higest pixel in MST.
  // Make key 0 so that this pixel is picked as first pixel
  int highestAddress = evt.highestPixelAddress();
  key[highestAddress] = 0;
  parent[highestAddress] = highestAddress; //This pixel is root of MST
  
  // Loop on all the pixels
  for (int index = 0; index < dim - 1; ++index)
  {
    /* Pick the minimum key pixel from the set of pixels not yet included
       in MST */
    int minKeyId = minKey(key, mstSet); //return -1 if no pixel is found
    /* A result -1 means all pixels not included in MST are under theshold
       or not adjacent to any included pixel. In that case the cluster is
       complete and we exit the loop. */
    if (minKeyId < 0)
      break;
    // Else add the picked pixel to the MST Set
    mstSet[minKeyId] = true;
 
    /* Update key value and pixel index of the adjacent pixels of
       the picked pixel. Consider only those pixels which are not yet
       included in MST. */
    for (int v = 0; v < dim; ++v){
      if (!mstSet[v]){ //mstSet[v] is false for pixels not yet included in MST
        /* Update the key only if distance from minKeyId is smaller than
           key[v] and is less than 2 (adjacent pixels).
           Ignore under threshold pixels. */
        if (evt(v).counts >= threshold){
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
  //    if (parent.at(index) >=0)
  //      std::cout << evt(index).counts << " ";
  //    else
  //      std::cout << "// ";
  //  }
  //  std::cout << std::endl;
  //}
  /******/  
  for (int i = 0; i < parent.size(); ++i){
     if (parent.at(i) >=0)
        m_hits.push_back(evt(i));
  }
}


int pCluster::minKey(const std::vector<int> &key,
                     const std::vector<bool> &mstSet) const
{
   // Initialize min value
   int min = INT_MAX;
   int min_index = -1;
 
   for (int v = 0; v < key.size(); v++){
     if (mstSet.at(v) == false && key.at(v) < min){
       min = key.at(v);
       min_index = v;
     }
   }
   return min_index;
}


std::ostream& pCluster::fillStream(std::ostream& os) const
{
  for (const event::Hit& hit : m_hits) // access by const reference
        os << "(" << hit.x << "-" << hit.y << ") -> " << hit.counts
           << std::endl;
  return os;
}
