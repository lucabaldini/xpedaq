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
                                   std::string usrComment) : 
  pRunController(configFilePath, preferencesFilePath, trgMaskFilePath,
    usrComment, true),
  m_compareWithRef(false),
  m_nSigmaAlarmThreshold(20),
  m_nBadPixelsThreshold(10),
  m_nCorruptedEvents(0),
  m_writtenDataBlocks(0),
  m_referenceMapFilePath("")
{
  m_pedestalMap = new PedestalsMap();
  connect (m_dataCollector, SIGNAL(blockRead(const pDataBlock&)),
          this, SLOT(readDataBlock(const pDataBlock&)));
}


/*!
 */
void pedRunController::loadRefMapFromFile(std::string referenceMapFilePath)
{
  m_compareWithRef = true;
  m_referenceMapFilePath = referenceMapFilePath;
  if (!xpedaqos::fileExists(m_referenceMapFilePath)) {
    *xpollog::kError << "File not found: " << m_referenceMapFilePath
                     << endline;
    exit(1);
  }
  if (!isReferenceMapPathValid(m_referenceMapFilePath)){
    *xpollog::kError << "Input file does not look like a valid .pmap file: "
                     << m_referenceMapFilePath << endline;
    exit(1);
  }
  *xpollog::kInfo << "Reading pedestals map from " << m_referenceMapFilePath
                  << "... " << endline;
  m_referenceMap = new PedestalsMap();
  std::ifstream *inputFile = xpolio::kIOManager->
                                        openInputFile(m_referenceMapFilePath);
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
void pedRunController::setNSigmaAlarmThreshold(int nSigma)
{
  *xpollog::kInfo << "Setting the threshold for bad pixel to "
                  << nSigma << " [sigma] ... " << endline;
  m_nSigmaAlarmThreshold = nSigma;
}


/*!
 */
void pedRunController::setNBadPixelsThreshold(int nBadPixels)
{
  *xpollog::kInfo << "Setting the number of bad pixel to consider an event "
                  << "corrupted to " << nBadPixels
                  << " ... " << endline;
  m_nBadPixelsThreshold = nBadPixels;
}


/*!
 */
std::string pedRunController::corruptedOutFilePath()
{
  return outputFilePath("corrupted_blocks.mdat");
}


/*!
 */
std::string pedRunController::pedMapOutFilePath() const
{
  return outputFilePath("pedMap.pmap");
}


/*!
 */
void pedRunController::readDataBlock(const pDataBlock &block)
{
  int curBlockCorruptedEvents = 0;
  // loop on pDataBlock events
  for (unsigned int evt = 0; evt < block.numEvents(); ++evt) { 
    unsigned int x = 1000; //unphysical initialization
    unsigned int y = 1000; //unphysical initialization
    adc_count_t height = 0;
    int curEventBadPixels = 0;
    // loop on the pixels of a single event
    for (unsigned int index = 0; index < block.numPixels(evt); ++index) {
      block.readPixel(evt, index, x, y, height);
      m_pedestalMap->fill(x, y, height);
      if (m_compareWithRef){ // comparison with reference map
        double dist = m_referenceMap->normDistance(x, y, height);
        if (dist > m_nSigmaAlarmThreshold){
          curEventBadPixels++;
          //*xpollog::kError << "Bad pixel at (" << x << "," << y << ")."
          //                 << " Value = " << height << " Reference is "
          //                 << m_referenceMap->average(x, y) << " +- "
          //                 << m_referenceMap->rms(x, y)
          //                 << ", Norm. distance = " << dist << " [sigma]"
          //                 << endline;
        }
      } // end of comparison with reference map
    } // end of the loop on single event pixels
    if (curEventBadPixels >= m_nBadPixelsThreshold){
      *xpollog::kInfo << "Corrupted data block with " << curEventBadPixels 
                      << " bad pixels" << endline;
      curBlockCorruptedEvents++;
    }
  } // end of the loop on events in the dataBlock
  if (m_compareWithRef && curBlockCorruptedEvents>0){
    m_nCorruptedEvents+=curBlockCorruptedEvents;
    writeCorruptedBlock(block);
  }
}


/*!
 */
void pedRunController::writeCorruptedBlock(const pDataBlock &block)
{
  m_writtenDataBlocks++;
  *xpollog::kInfo << "Writing corrupted data block n. " << m_writtenDataBlocks
  << endline;
  std::ofstream *outputFile = xpolio::kIOManager->
                           openOutputFile(corruptedOutFilePath(), true, true);
  outputFile->write(block.getCharDataBlock(), block.size());
  xpolio::kIOManager->closeOutputFile(outputFile);
}


/*!
*/
void pedRunController::writeRunSummary()
{
  pRunController::writeRunSummary();
  if (m_compareWithRef){
    *xpollog::kInfo << "Reference pedestals map: " << referenceMapFilePath()
                  << endline;
    *xpollog::kInfo << "Found " << numCorruptedEvents() << " events with "
                    << "at least " << m_nBadPixelsThreshold << " pixels "
                    << "more than " << m_nSigmaAlarmThreshold << " sigma "
                    << "away from the reference average." << endline;
  }
}


/*!
 */
void pedRunController::writeMapToFile() const
{
  *xpollog::kInfo << "Writing pedestals map to " << pedMapOutFilePath()
                  << "... " << endline;
  std::ofstream *outputFile = xpolio::kIOManager->
    openOutputFile(pedMapOutFilePath(), true, true);
  std::string headerLine = "#" + pedMapOutFilePath();
  xpolio::kIOManager->write(outputFile, headerLine, true);
  writeNevents(outputFile);
  writeMap(outputFile);
  xpolio::kIOManager->closeOutputFile(outputFile); 
}


/*!
 */
void pedRunController::writeNevents(std::ofstream *outputFile) const
{
  xpolio::kIOManager->write(outputFile, "#N. events", true);
  xpolio::kIOManager->write(outputFile, m_pedestalMap->numEntries(0,0), true);
}


/*!
 */
void pedRunController::writeMap(std::ofstream *outputFile, int precision,
                                std::string pixelSeparator,
                                std::string valueSeparator) const
{
  std::string mapHeaderLine  = "#Map (average " + valueSeparator + "rms)";
  xpolio::kIOManager->write(outputFile, mapHeaderLine, true);
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
void pedRunController::writeRunStat(std::string filePath) const
{
  //If we are not counting the corrupted events, the base version is invoked
  if (!m_compareWithRef){
    pRunController::writeRunStat(filePath);
  } else {
  //Otherwise we use the derived version, with more info. Code duplication is
  //ugly, but the idea is not to mess to much with the base class.
    std::ofstream *outputFile = xpolio::kIOManager->openOutputFile(filePath);
    *outputFile << "Start date/time: " << startDatetime() << std::endl;
    *outputFile << "Start seconds: " << m_startSeconds  << std::endl;
    *outputFile << "Stop date/time: " << stopDatetime()  << std::endl;
    *outputFile << "Stop seconds: " << m_stopSeconds  << std::endl;
    *outputFile << "Run duration [s]: " << runDuration() << std::endl;
    *outputFile << "Number of events: " << numEvents()  << std::endl;
    *outputFile << "Number of data blocks: " << numDataBlocks()  << std::endl;
    *outputFile << "Number of corrupted events: " << numCorruptedEvents()
                << std::endl;
    xpolio::kIOManager->closeOutputFile(outputFile);
  }
}


/*!
 */
void pedRunController::resetPedMap()
{
  m_pedestalMap->reset();
}


/*!
 */
void pedRunController::reset()
{
  m_nCorruptedEvents = 0;
  m_writtenDataBlocks = 0;
  resetPedMap();  
}
