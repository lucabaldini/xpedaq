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

#ifndef PEDDATAFILE_H
#define PEDDATAFILE_H

#include "pedFile.h"
#include "pDataBlock.h"

class PedDataFile: public PedFile 
{
  public:
    PedDataFile(std::string filePath);
    
    /* \brief Fill the given map with the events in the file
    */
    virtual int fillPedMap(PedestalsMap& map) const;
    
    /* \brief Fill the given map with the events in a specific subrange of the
              file
    */
    virtual int fillPedMap(PedestalsMap& map, int firstEvent,
                           int numEvents = 1) const;                  
    
    /* \brief Fill the given map with the events at the positions specified
              in the input array.
    */
    virtual void fillPedMap(PedestalsMap& map,
                            const std::vector<int>& events) const;
    
    /* \brief Return the event number of the event currently pointed by the
              read cursor of the file streamer.
    */
    int curEvent() const;
  
  private:
    
    virtual void readNumberOfEvents();
    void goToEvent(int evtNumber) const;
    void addEventToMap(PedestalsMap& map, int evtNumber) const;
    void addNextEventToMap(PedestalsMap& map) const;
    void addDataBlockToMap(PedestalsMap& map, const pDataBlock &block) const;
    int nBytesPerEvent() const {return 2*xpoldetector::kNumPixels;}
};

#endif //PEDDATAFILE_H
