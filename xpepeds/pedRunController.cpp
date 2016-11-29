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

#include "pedRunController.h"

/*!
 */
pedRunController::pedRunController(std::string configFilePath,
                                   std::string preferencesFilePath,
                                   std::string trgMaskFilePath,
                                   std::string referenceMapFilePath,
                                   double nSigmaAlarmThreshold,
                                   std::string usrComment) : 
  pRunController(configFilePath, preferencesFilePath, trgMaskFilePath,
    usrComment, true),
  m_compareWithRef(false),
  m_nSigmaAlarmThreshold(nSigmaAlarmThreshold)
{
  m_pedestalMap = new PedestalsMap();
  if (!referenceMapFilePath.empty()){
    m_compareWithRef = true;
    loadRefMapFromFile(referenceMapFilePath);
  }  
  connect (m_dataCollector, SIGNAL(blockRead(const pDataBlock&)),
          this, SLOT(readDataBlock(const pDataBlock&)));
}                                   


/*!
 */
void pedRunController::readDataBlock(const pDataBlock &p)
{
  for (unsigned int evt = 0; evt < p.numEvents(); ++evt) {   
    unsigned int x = 1000; //unphysical initialization
    unsigned int y = 1000; //unphysical initialization
    adc_count_t height = 0;
    for (unsigned int index = 0; index < p.numPixels(evt); ++index) {
      p.readPixel(evt, index, x, y, height);
      if (m_compareWithRef){
        double dist = m_referenceMap->normDistance(x, y, height);
      }
      //if (dist > m_nSigmaAlarmThreshold){
      //  *xpollog::kError << "Outlier pixel at (" << x << "," << y << ")."
      //                  << " Value = " << height << ", Norm. distance = "
      //                   << dist << " [sigma]" << endline;
      //}
      m_pedestalMap->fill(x, y, height);
    }
  }	
}


/*!
 */
void pedRunController::loadRefMapFromFile(std::string referenceMapFilePath)
{
  if (!xpedaqos::fileExists(referenceMapFilePath)) {
    *xpollog::kError << "File not found: " << referenceMapFilePath
                     << endline;
    exit(1);
  }
  if (!isReferenceMapPathValid(referenceMapFilePath)){
    *xpollog::kError << "Input file does not appear to be a valid .pmap file: "
                     << referenceMapFilePath << endline;
    exit(1);
  }
  *xpollog::kInfo << "Reading pedestals map from " << referenceMapFilePath
                  << "... " << endline;
  m_referenceMap = new PedestalsMap();
  std::ifstream *inputFile = xpolio::kIOManager->
                                           openInputFile(referenceMapFilePath);
  // Skip the header
  xpolio::kIOManager->skipLine(inputFile);
  // Skip the number of events header
  xpolio::kIOManager->skipLine(inputFile);
  // Read the number of events
  int nEntries = xpolio::kIOManager->readInteger(inputFile);
  *xpollog::kInfo << "Reference map has " << nEntries << " events"
                  << endline;
  // Skip the map header
  xpolio::kIOManager->skipLine(inputFile);
  // Read average and rms for each pixel and fill the reference map
  for (unsigned int row=0; row < pedestals::kNrow; ++row){
    for (unsigned int col=0; col < pedestals::kNcol; ++col){
      double average = xpolio::kIOManager->readDouble(inputFile, false);
      double rms = xpolio::kIOManager->readDouble(inputFile, false);
      m_referenceMap->setPixel(col, row, nEntries, average, rms);
    }
    inputFile->ignore(1, '\n');
  }
  xpolio::kIOManager->closeInputFile(inputFile);
  *xpollog::kInfo << "Done." << endline;
}


/*! Check if the input file name ends with ".pmap"
*/
bool pedRunController::isReferenceMapPathValid(
  std::string referenceMapFilePath) const
{
  std::string ending = ".pmap";
  if (referenceMapFilePath.length() >= ending.length()) {
    return (0 == referenceMapFilePath.compare (referenceMapFilePath.length()
      - ending.length(), ending.length(), ending));
  } else {
    return false;
  }
}


/*!
 */
std::string pedRunController::pedMapOutFilePath() const
{
  return outputFilePath("pedMap.pmap");
}


/*!
 */
void pedRunController::writeMapToFile() const
{
  std::ofstream *outputFile = xpolio::kIOManager->
    openOutputFile(pedMapOutFilePath(), true, true);
  std::string headerLine = "# Some header";
  xpolio::kIOManager->write(outputFile, headerLine, true);
  writeNevents(outputFile);
  writeMap(outputFile);
  xpolio::kIOManager->closeOutputFile(outputFile);
}


/*!
 */
void pedRunController::writeNevents(std::ofstream *outputFile) const
{
  xpolio::kIOManager->write(outputFile, "# N. events", true);
  xpolio::kIOManager->write(outputFile, m_pedestalMap->numEntries(0,0), true);
}


/*!
 */
void pedRunController::writeMap(std::ofstream *outputFile, int precision,
                                std::string pixelSeparator,
                                std::string valueSeparator) const
{
  xpolio::kIOManager->write(outputFile, "# Map", true);
  for (unsigned int row=0; row < pedestals::kNrow; ++row){
    for (unsigned int col=0; col < pedestals::kNcol; ++col){
      double ave = m_pedestalMap->average(col, row);
      double rms = m_pedestalMap->rms(col, row);
      xpolio::kIOManager->write(outputFile, ave, false, true, precision);
      xpolio::kIOManager->write(outputFile, valueSeparator, false);
      xpolio::kIOManager->write(outputFile, rms, false, true, precision);
      xpolio::kIOManager->write(outputFile, pixelSeparator, false);
    }
    *outputFile << std::endl;
  }
}


/*!
 */
void pedRunController::resetPedMap()
{
  m_pedestalMap->reset();
}
