#ifndef PCLUSTER_H
#define PCLUSTER_H

#include <limits.h>
#include <iostream>
#include <vector>
#include "pEvent.h"

class pCluster
{
  public:
    
    pCluster(const pEvent &evt, int threshold);
  
    //getters
    const event::Hit& operator() (int index) const {return m_hits.at(index);}  //access by index number
    
    // Terminal formatting.
    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const pCluster& clst)
      {return clst.fillStream(os);}
    
  private:
    
    int minKey(const std::vector<int> &key, 
               const std::vector<bool> &mstSet) const;
    
    std::vector<event::Hit> m_hits;
    
};

#endif //PCLUSTER_H
