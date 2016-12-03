/***********************************************************************
Copyright (C) 2007, 2008 by Luca Baldini (luca.baldini@pi.infn.it),
Johan Bregeon, Massimo Minuti and Gloria Spandre.

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

#include "pDataCollector.h"


pDataCollector::pDataCollector(pXpolFpga *xpolFpga, bool emitBlocks,
			                         int thresholdUpdateInterval):
  m_xpolFpga(xpolFpga),
  m_thresholdUpdateInterval(thresholdUpdateInterval),
  m_numMalformedBlocks(0),
  m_emitBlocks(emitBlocks),
  m_numWrongRoiEvents(0)
{ 
  //Register pDataBlock as object that can be emitted as signals
  qRegisterMetaType<pDataBlock>("pDataBlock");
  m_lastThresholdUpdate = 0;
}


/*! Return the current system time in seconds.
 */
long int pDataCollector::currentSeconds() const
{
  return static_cast<long int> (time(NULL));
}


/*! Called by the run controller in pRunController::init() and
  pRunController::fsmStartRun(); namely at beginning and each time the
  start button is pressed.

  \todo Don't think
  \verbatim
  m_running = false;
  \endverbatim
  should be called, here. Only in the constructor and in
  pDataCollector::stop().
*/

void pDataCollector::reset()
{
  m_running = false;
}

void pDataCollector::stop()
{
  m_running = false;
}


/*!
  This is actually the core of the data acquisition. 
*/
void pDataCollector::run()
{
  // Force a threshold update upon start run.
  m_lastThresholdUpdate = 0;
  m_dataFIFO = new pDataFIFO(m_outputFilePath, m_userPreferences);
  m_numMalformedBlocks = 0;
  m_numWrongRoiEvents = 0;
  m_running = true; 
  long unsigned int fullFrameDataBufferDimension = SRAM_DIM*2; // this definition should definitely be somewhere else
  long unsigned int maxSize = m_detectorConfiguration->maxBufferSize();
  long unsigned int bufferDimension = fullFrameDataBufferDimension;
  pDataBlock *curDataBlock;
  m_xpolFpga->usbController()->resetSequencer();
  m_xpolFpga->usbController()->startSequencer();
  int errorCode = 0;
  unsigned short pixAddressX = m_detectorConfiguration->pixelAddressX();
  unsigned short pixAddressY = m_detectorConfiguration->pixelAddressY();
  bool writeToDisk = m_userPreferences->dataFileEnabled();
  while (m_running) {
    unsigned char* dataBuffer = new (std::nothrow) 
      unsigned char[bufferDimension];
    if (dataBuffer == nullptr){
      *xpollog::kError << "Allocation failed" << endline;
      m_running = false;
      // now that the buffer allocation is inside the cycle this check is
      // not sufficient to avoid a bad crash. Need a proper exception. 
    } 
    errorCode = m_xpolFpga->usbController()->readData(dataBuffer,
						      &bufferDimension);
    if (errorCode) {
      m_running = false;
    } else {
      if (m_fullFrame) {
	curDataBlock = new pDataBlock(dataBuffer);
      } else {
	curDataBlock = new pDataBlock(dataBuffer, maxSize);

	// Shit to check the ROI.
	if (m_detectorConfiguration->readoutMode() ==
	    xpoldetector::kChargeInjectionReadoutCode) {
	  for (unsigned int evt = 0; evt < curDataBlock->numEvents(); evt ++) {
	    int errorCode = curDataBlock->verifyRoi(evt, pixAddressX,
						    pixAddressY);
	    if (errorCode) {
	      m_numWrongRoiEvents++;
	      *xpollog::kError << "Window mismatch at event "
			       << m_dataFIFO->getNumAcquiredEvents()
			       << " (error code 0x" << errorCode 
			       << ")" << endline;
	    }
	  }
	}
	// End of shit.
	
      }
      if (curDataBlock->errorSummary()) {
	*xpollog::kError << "Data block at index " 
			 << m_dataFIFO->getNumAcquiredEvents()
			 << "+ has error summary 0x" << hex 
                         << curDataBlock->errorSummary() << dec << "."
                         << endline;
	std::cerr << *curDataBlock << std::endl;
        dumpRawBuffer(dataBuffer);
	m_numMalformedBlocks ++;
      } else {
	      if (m_emitBlocks){
	        emit blockRead(*curDataBlock);
	      }
        m_dataFIFO->fill(curDataBlock);
        m_dataFIFO->setStartSeconds(m_startSeconds);
        m_dataFIFO->flush(writeToDisk);
      }
      delete curDataBlock;
      // Fist attempt at correcting for the vref drift as a function of time,
      // see https://github.com/lucabaldini/xpedaq/issues/137
      // (note that vref is read and accounted for in the function call to
      // pXpolFpga::setDacThreshold().)
      long int seconds = currentSeconds();
      if (!m_fullFrame && 
	  seconds - m_lastThresholdUpdate >= m_thresholdUpdateInterval) {
	m_xpolFpga->setDacThreshold(m_detectorConfiguration);
	m_lastThresholdUpdate = seconds;
      }
    }
  }
  m_xpolFpga->usbController()->stopSequencer();
  m_xpolFpga->usbController()->flushQUsbFIFO();
  m_xpolFpga->usbController()->resetSequencer();
  m_xpolFpga->usbController()->readUsbSettings();
  m_xpolFpga->usbController()->writeUsbSettings();
  m_xpolFpga->usbController()->readUsbSettings();
  delete m_dataFIFO;
}


/*! This is needed since the data collector needs run based information.
  This information must be provided each time the start button is
  pressed (not once and forever at the construction time).
*/
void pDataCollector::setupRun(std::string outputFilePath,
                              long int startSeconds,
                              pUserPreferences *preferences,
                              pDetectorConfiguration *configuration)
{
  m_outputFilePath = outputFilePath;
  m_startSeconds = startSeconds;
  m_userPreferences = preferences;
  m_detectorConfiguration = configuration;
  m_fullFrame = (m_detectorConfiguration->readoutMode() ==
		 xpoldetector::kFullFrameReadoutCode);
}


void pDataCollector::dumpRawBuffer(unsigned char *buffer)
{
  std::stringstream filePath("");
  filePath << m_outputFilePath << ".error" <<  m_numMalformedBlocks;
  std::ofstream *outputFile = xpolio::kIOManager->
    openOutputFile(filePath.str(), true, true);
  outputFile->write(reinterpret_cast<char *>(buffer), SRAM_DIM*2);
  xpolio::kIOManager->closeOutputFile(outputFile);
}
