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

#ifndef PEDFILE_H
#define PEDFILE_H

#include <string>
#include <vector>

#include "xpollog.h"
#include "xpedaqos.h"
#include "xpolio.h"
#include "xpoldetector.h"

#include "pedestalsMap.h"

class PedFile
{
  public:
    /* \brief Base constructor
    */
    PedFile(std::string filePath);
    
    /* \brief Base destructor (it needs to be virtual http://www.learncpp.com/cpp-tutorial/123-virtual-destructors-virtual-assignment-and-overriding-virtualization/)
    */
    virtual ~PedFile();
    
    /* \brief Fill the given map with the events in the file (pure virtual)
    */
    virtual int fillPedMap(PedestalsMap& map) const = 0;
    
    /* \brief Fill the given map with the events in a specific subrange of the
              file (pure virtual)
    */
    virtual int fillPedMap(PedestalsMap& map, int firstEvent,
      int numEvents =1) const = 0;
    
    /* \brief Fill the given map with the events at the positions specified
              in the input array (pure virtual)
    */
    virtual void fillPedMap(PedestalsMap& map,
                            const std::vector<int>& events) const = 0;
    
    /* \brief Number of events in the file
    */
    int nEvents() const {return m_nEvents;}
   
    enum inputFileType {
      undefined,
      dataType, // file ".mdat"
      pedmapType, // file ".pmap"
      unknownType
    };

    inputFileType fileType() const {return m_fileType;}
    std::streampos fileSize() const;
    
  protected:   
    /*! \brief Read the number of events from the file
    */
    virtual void readNumberOfEvents() = 0;
    
    /*! \brief Move the read cursor to the beginning of the file
    */
    void goToStart() const;
    
    /*! \brief Input file
    */
    std::ifstream* m_inputFile;
    
    /*! \brief File type
    */
    inputFileType m_fileType;
    
    /*! \brief Number of events
    */
    int m_nEvents;
};

extern PedFile::inputFileType identifyFileType(std::string filePath);

#endif //PEDFILE_H
