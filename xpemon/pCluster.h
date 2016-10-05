#ifndef PCLUSTER_H
#define PCLUSTER_H

#include "pEventWindow.h"

/* Class describing a cluster in an event. Basically it is a std::vector of
   bool, of the same size of the event, telling if the hit at that position
   belong to the cluster or not */

class pCluster
{
  public:
    
    pCluster() {;}
    
    void build(const pEventWindow &evt, const std::vector<event::Hit> &hits,
               int threshold);
      
    //getters
    bool operator() (int index) const {return m_isInCluster.at(index);}
    
    // Terminal formatting.
    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const pCluster& clst)
      {return clst.fillStream(os);}
    
  private:
    
    int minKey(const std::vector<int> &key) const;
     
    std::vector<bool> m_isInCluster;
    
};

#endif //PCLUSTER_H
