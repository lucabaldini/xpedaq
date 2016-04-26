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


pDataCollector::pDataCollector(pUsbController *usbController):
  m_usbController(usbController)  
{ 

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
  This is actually the core of the data acquisition. When the method is called:
  \li A new \ref pDataFIFO object with the correct runtime parameters is created.
  \li The FPGA is reset and started.
  \li The event loop is started; each time a data block is read out either a
  \ref pDataCollector::readoutErrorDetected() signal is emitted (if there is a
  readout error) or the data FIFO is filled.
*/

void pDataCollector::run()
{
  time_t timer;
  time(&timer);
  m_startSeconds = (unsigned int)timer;
  *xpollog::kInfo << "Start time = " << m_startSeconds << " s." << endline;
  m_dataFIFO = new pDataFIFO(m_outputFilePath, m_userPreferences);
  unsigned long dataBufferDimension = SRAM_DIM*2;
  unsigned char dataBuffer[SRAM_DIM*2];
  m_usbController->resetSequencer();
  m_usbController->startSequencer();
  m_running = true;
  int errorCode = 0;
  while (m_running) {
    errorCode = m_usbController->readData(dataBuffer, &dataBufferDimension);
    if (errorCode) {
      m_running = false;
      emit readoutErrorDetected(errorCode);
    } else {
      if (m_fullFrame) {
	m_dataFIFO->fill(new pDataBlock(dataBuffer));
      } else {
	m_dataFIFO->fill(new pDataBlock(dataBuffer,
	    m_detectorConfiguration->getMaxBufferSize()));
      }
      m_dataFIFO->flush();
    }
  }
  m_usbController->stopSequencer();
  m_usbController->flushQUsbFIFO();
  m_usbController->resetSequencer();

  m_usbController->readUsbSettings();
  m_usbController->writeUsbSettings();
  m_usbController->readUsbSettings();
  delete m_dataFIFO;
}

/*! This is needed since the data collector needs run based information.
  This information must be provided each time the start button is
  pressed (not once and forever at the construction time).
*/

void pDataCollector::setup(std::string outputFilePath,
			   pUserPreferences *preferences,
			   pDetectorConfiguration *configuration)
{
  m_outputFilePath = outputFilePath;
  m_userPreferences = preferences;
  m_detectorConfiguration = configuration;
  m_fullFrame = (m_detectorConfiguration->getReadoutMode() ==
		 xpoldetector::kFullFrameReadoutCode);
}
