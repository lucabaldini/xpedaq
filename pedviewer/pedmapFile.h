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

#ifndef PEDMAPFILE_H
#define PEDMAPFILE_H

#include "pedFile.h"

class PedmapFile: public PedFile 
{
  public:
    PedmapFile(std::string filePath);
    virtual int fillPedMap(PedestalsMap& map) const;
  
  private:
    /* These inherited fucntions do not really make sense for this class, so
       we make them private (we still need a definition, because they are
       purely virtual in the base class) */
    virtual int fillPedMap(PedestalsMap&, int, int) const {return -1;}
    virtual void fillPedMap(PedestalsMap&, const std::vector<int>&) const
      {return;}
    virtual void readEvent (char*, int) const {return;}
    virtual void readNumberOfEvents();
       
};

#endif //PEDMAPFILE_H
