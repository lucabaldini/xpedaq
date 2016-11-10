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


pDataCollector::pDataCollector(pUsbController *usbController, bool emitBlocks):
  m_usbController(usbController),
  m_numMalformedBlocks(0),
  m_emitBlocks(emitBlocks)
{ 
  //Register pDataBlock as object that can be emitted as signals
  qRegisterMetaType<pDataBlock>("pDataBlock");
  // Setup the timer to update the vref.
  m_timer = new QTimer();
  m_timer->setInterval(60000);
  m_timer->setSingleShot(true);
  connect(this, SIGNAL(thresholdUpdated()), m_timer, SLOT(start()));
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
  m_timer->stop();
}

void pDataCollector::stop()
{
  m_running = false;
  m_timer->stop();
}

/*!
  This is actually the core of the data acquisition. 
*/

void pDataCollector::run()
{
  m_dataFIFO = new pDataFIFO(m_outputFilePath, m_userPreferences);
  m_numMalformedBlocks = 0;
  m_running = true;
  m_timer->start();
  unsigned long dataBufferDimension = SRAM_DIM*2;  
  unsigned char* dataBuffer = new (std::nothrow) unsigned char[dataBufferDimension];
  if (dataBuffer == nullptr)
  {
    std::cout << "allocation failed" << std::endl;
    m_running = false;
    m_timer->stop();
  }  
  int maxSize = m_detectorConfiguration->maxBufferSize();
  pDataBlock *curDataBlock;
  m_usbController->resetSequencer();
  m_usbController->startSequencer();
  int errorCode = 0;
  while (m_running) {
    errorCode = m_usbController->readData(dataBuffer, &dataBufferDimension);
    if (errorCode) {
      m_running = false;
      m_timer->stop();
    } else {
      if (m_fullFrame) {
	curDataBlock = new pDataBlock(dataBuffer);
      } else {
	curDataBlock = new pDataBlock(dataBuffer, maxSize);
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
	if (m_emitBlocks) emit blockRead(*curDataBlock);
	m_dataFIFO->fill(curDataBlock);
	m_dataFIFO->setStartSeconds(m_startSeconds);
	m_dataFIFO->flush();
      }
      delete curDataBlock;
      // Fist attempt at correcting for the vref drift as a function of time,
      // see https://github.com/lucabaldini/xpedaq/issues/137
      // (note that vref is read and accounted for in the function call to
      // pXpolFpga::setDacThreshold().)
      // We're creating a pointer to a pXpolFpga object, here, while in
      // principle we could pass downstream the one in the run controlerr.
      // If we end up keeping this we might want to cleanup things.
      // Also note that we only want to do this when operating in window mode.
      if (!m_fullFrame && !m_timer->isActive()) {
	pXpolFpga xpol(m_usbController);
	xpol.setDacThreshold(m_detectorConfiguration);
	emit thresholdUpdated();
      }
    }
  }
  m_usbController->stopSequencer();
  m_usbController->flushQUsbFIFO();
  m_usbController->resetSequencer();
  m_usbController->readUsbSettings();
  m_usbController->writeUsbSettings();
  m_usbController->readUsbSettings();
  delete m_dataFIFO;
  delete [] dataBuffer;
}

/*! This is needed since the data collector needs run based information.
  This information must be provided each time the start button is
  pressed (not once and forever at the construction time).
*/

void pDataCollector::setupRun(std::string outputFilePath, long int startSeconds,
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
