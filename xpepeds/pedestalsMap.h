#ifndef PEDESTALSMAP_H
#define PEDESTALSMAP_H

#include <vector>
#include <iostream>

#include "xpoldetector.h"
#include "pRunningStat.h"


namespace pedestals{
  const unsigned int kNx = static_cast<unsigned int>(xpoldetector::kNumPixelsX);
  const unsigned int kNy = static_cast<unsigned int>(xpoldetector::kNumPixelsY);
  const unsigned int kNPedestal = kNx * kNy;
}

/* This is the core object of the pedestal application. It's a 2D map of
   pRunningStat object (one for each pixel of the detector), holding the
   mean and the variance of the respective pixels.
   Internally the map is implemented as a one dimensional vector, with a method
   for transforming (x,y) coordinates into a single array index */

class PedestalsMap
{ 

  public:
  
    PedestalsMap();
    
    //Getters
    int numValues (unsigned int pixelX, unsigned int pixelY) const;
    double average(unsigned int pixelX, unsigned int pixelY) const;
    double variance(unsigned int pixelX, unsigned int pixelY) const;
    double rms(unsigned int pixelX, unsigned int pixelY) const;  

    void fill(unsigned int pixelX, unsigned int pixelY, double value);
	void reset();

    pRunningStat& operator()(unsigned int pixelX, unsigned int pixelY);
    const pRunningStat& operator()(unsigned int pixelX,
                                   unsigned int pixelY) const;
    
  private:
  
    pRunningStat& pedestal(unsigned int pixelX, unsigned int pixelY);
    const pRunningStat& pedestal(unsigned int pixelX,
                                 unsigned int pixelY) const;
    unsigned int binIndex (unsigned int pixelX, unsigned int pixelY) const;
    
    std::vector<pRunningStat> m_pedMap;

};

#endif // PEDESTALSMAP_H
