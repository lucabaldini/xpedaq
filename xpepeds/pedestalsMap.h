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

#ifndef PEDESTALSMAP_H
#define PEDESTALSMAP_H

#include <vector>
#include <iostream>
#include <string>

#include "xpoldetector.h"
#include "pRunningStat.h"


/* This is the core object of the pedestal application. It's a 2D map of
   pRunningStat object (one for each pixel of the detector), each holding the
   mean and the variance of the respective pixel.
   Internally the map is implemented as a one dimensional vector, with a method
   for transforming (x,y) coordinates into a single array index */

class PedestalsMap
{ 

  public:
  
    PedestalsMap();
        
    //Getters
    int numEntries (unsigned int pixelX, unsigned int pixelY) const;
    double average(unsigned int pixelX, unsigned int pixelY) const;
    double variance(unsigned int pixelX, unsigned int pixelY) const;
    double rms(unsigned int pixelX, unsigned int pixelY) const;  
    double normDistance(unsigned int pixelX, unsigned int pixelY,
                        double value) const;

    void fill(unsigned int pixelX, unsigned int pixelY, double value);
	  void setPixel(unsigned int pixelX, unsigned int pixelY, int nEntries,
	                double average, double rms);
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
