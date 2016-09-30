#ifndef PCLUSTER_H
#define PCLUSTER_H

#include <limits.h>
#include <vector>
#include "pEvent.h"

class pCluster
{
  public:
    
    pCluster(const pEvent &evt, int threshold);
  
  private:
    
    int minKey(const std::vector<int> &key, 
               const std::vector<bool> &mstSet) const;
    
    std::vector<event::Hit> m_hits;
    
};

#endif //PCLUSTER_H
