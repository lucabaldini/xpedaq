// A C++ program for Prim's Minimum Spanning Tree (MST) algorithm. 
// http://www.geeksforgeeks.org/greedy-algorithms-set-5-prims-minimum-spanning-tree-mst-2/
 
#include <stdio.h>
#include <limits.h>
#include "pEvent.h"

// A utility function to find the pixel with minimum key value, from
// the set of pixels not yet included in MST
int minKey(const std::vector<int> &key, const std::vector<bool> &mstSet)
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

// A utility function to print the constructed MST stored in parent[]
void printMST(const std::vector<int> &parent, const pEvent &evt)
{
   for (int i = 0; i < parent.size(); ++i){
     if (parent.at(i) >=0)
        std::cout << i << std::endl;
   }
}


void primMST(const pEvent &evt, int threshold)
{
  int dim = evt.evtSize();
  std::vector<int> parent; // Array to store constructed MST
  std::vector<int> key;   // Key values used to pick minimum weight edge in cut
  std::vector<bool>  mstSet;  // To represent set of pixels included in MST
  
  // Initialize all keys as INFINITE
  for (int i = 0; i < dim; ++i){
    key.push_back(INT_MAX);
    mstSet.push_back(false);
    parent.push_back(-1); // non physical init
  }
     
  // Start from higest pixel in MST.
  // Make key 0 so that this pixel is picked as first pixel
  key[evt.highestPixel()] = 0;
  parent[evt.highestPixel()] = evt.highestPixel(); //This pixel is root of MST

  // Loop on all the pixels
  for (int count = 0; count < dim - 1; ++count)
  {
    // Pick the minimum key pixel from the set of pixels
    // not yet included in MST
    int u = minKey(key, mstSet); //return -1 if no pixel is found
    // A result -1 means all pixels not included in MST are under theshold
    // or not adjacent to any included pixel. In that case we exit.
    if (u < 0)
      break;
    // Else add the picked pixel to the MST Set
    mstSet[u] = true;
 
    // Update key value and parent index of the adjacent pixels of
    // the picked pixel. Consider only those pixels which are not yet
    // included in MST
    for (int v = 0; v < dim; v++){
      if (!mstSet[v]){ //mstSet[v] is false for pixels not yet included in MST
        // Update the key only if dist(u,v) is smaller than key[v], less than
        // 2 (adjacent pixel) and the pixel is over threshold
        int dist = evt.cubeDistance(u, v);
        if (dist && dist < 2 && dist <  key[v] && evt(v) >= threshold){
          parent[v]  = u;
          key[v] = dist;    
        }
      }
    }
  }
  // print the constructed MST
  printMST(parent, evt);
  return;
}
