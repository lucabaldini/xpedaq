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

#ifndef PEDRUNCONTROLLER_H
#define PEDRUNCONTROLLER_H

#include <iostream>

#include "xpolio.h"
#include "pRunController.h"
#include "pedestalsMap.h"


/* Run controller for the pedestal application.
   It is basically identical to its base class pRunController, but with an extra
   member for keeping track of the value and rms of each pixel, and a method for
   filling it from a pDataBlock.
*/ 
   
class pedRunController: public pRunController
{

  Q_OBJECT 
   
  public:
    
    pedRunController(std::string configFilePath,
                     std::string preferencesFilePath,
                     std::string trgMaskFilePath,
                     std::string referenceMapFilePath,
                     double nSigmaAlarmThreshold = 10.,
                     std::string usrComment="");

    const PedestalsMap& pedMap() const {return (*m_pedestalMap);}

  
  public slots:
  
    void readDataBlock(const pDataBlock &p);
    void resetPedMap();
    void loadRefMapFromFile(std::string referenceMapFilePath);
    
    ///\brief Write average and rms map to file
    void writeMapToFile() const;

  private:
  
    ///\brief Returns the path to the pedestals map output file
    std::string pedMapOutFilePath() const;
    
    ///\brief Write the number of events into an open ouput file
    void writeNevents(std::ofstream *outputFile) const;
    
    ///\brief Write the average map to an open ouput file
    void writeMap(std::ofstream *outputFile, int precision = 2,
                  std::string pixelSeparator = " ",
                  std::string valueSeparator = " ") const;

    ///\brief Current map
    PedestalsMap *m_pedestalMap;
    
    ///\brief Reference map
    PedestalsMap *m_referenceMap;
    
    ///\brief Distance (in sigma) between the current content of a pixel and 
    // its refernce value required to consider it an outlier 
    double m_nSigmaAlarmThreshold;
};

#endif //PEDRUNCONTROLLER_H
