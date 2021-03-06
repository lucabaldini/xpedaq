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
                     std::string usrComment="");

    const PedestalsMap& pedMap() const {return (*m_pedestalMap);}
    int numCorruptedEvents() const {return m_nCorruptedEvents;}
    int numSigmaAlarmThreshold() const {return m_nSigmaAlarmThreshold;}
    int numBadPixelsThreshold() const {return m_nBadPixelsThreshold;}
    int getNumWrittenDataBlocks() const {return m_writtenDataBlocks;}
    int firstColumnExcluded() const {return m_excludeFirstColumn;}
    std::string referenceMapFilePath() const {return m_referenceMapFilePath;}
    
  public slots:
  
    void readDataBlock(const pDataBlock &p);
    void reset();
    void resetPedMap();
    void loadRefMapFromFile(std::string referenceMapFilePath);  
    int nSigmaAlarmThreshold() const {return m_nSigmaAlarmThreshold;}
    int nBadPixelsThreshold() const {return m_nSigmaAlarmThreshold;}
    void setNSigmaAlarmThreshold(int nSigma);
    void setNBadPixelsThreshold(int nBadPixels);
    void setFirstColumnExcluded(bool isExcluded)
      {m_excludeFirstColumn = isExcluded;}

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

    ///\brief Write the relevant run statistics to a file.
    virtual void writeRunStat(std::string filePath) const;
    
    ///\brief Write a short summary in the log file
    virtual void writeRunSummary();
    
    ///\brief Check the inut reference map file path
    bool isReferenceMapPathValid(std::string referenceMapFilePath) const;
    
    ///\brief Dump corrupted data block to file
    void writeCorruptedBlock(const pDataBlock &block);  
        
    ///\brief Return the path to the file with the corrupted data blocks
    std::string corruptedOutFilePath();
       
    ///\brief Current map
    PedestalsMap *m_pedestalMap;
    
    ///\brief Flag for comparing with reference
    bool m_compareWithRef;
    
    ///\brief Reference map
    PedestalsMap *m_referenceMap;
    
    ///\brief Distance (in sigma) between the current content of a pixel and 
    // its refernce value required to consider it a bad pixel 
    int m_nSigmaAlarmThreshold;
    
    ///\brief Number of bad pixels required to consider an event corrupted
    int m_nBadPixelsThreshold;
    
    ///\brief Number of corrupted events
    int m_nCorruptedEvents;
    
    ///\brief The number of blocks written to file.
    int m_writtenDataBlocks;
    
    ///\brief Path to the reference map file.
    std::string m_referenceMapFilePath;
    
    ///\brief Max number of bad pixels for event to print in the log
    int m_maxNumBadPixelsInLog;
    
    ///\brief Flag to not count bad pixels in first (noisy) column
    bool m_excludeFirstColumn;
};

#endif //PEDRUNCONTROLLER_H
