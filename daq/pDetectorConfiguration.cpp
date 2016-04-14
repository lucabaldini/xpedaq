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

#include "pDetectorConfiguration.h"

pDetectorConfiguration::pDetectorConfiguration()
{

}

void pDetectorConfiguration::setReadoutMode(unsigned short int mode)
{
  if (mode == xpoldetector::kFullFrameReadoutCode ||
      mode == xpoldetector::kWindowedReadoutCode  ||
      mode == xpoldetector::kChargeInjectionReadoutCode){
    m_readoutMode = mode;
  } else {
    *xpollog::kError << "Readout mode not defined." << endline;
    exit(1);
  }
}

void pDetectorConfiguration::setThresholdDac(int clusterId,
					     unsigned short int dacSetting)
{
  if (dacSetting > xpoldetector::kThresholdDacMax){
    m_thresholdDac[clusterId] = xpoldetector::kThresholdDacMax;
    *xpollog::kWarning << "Threshold out of range." << endline;
  } else if (dacSetting < xpoldetector::kThresholdDacMin){
    m_thresholdDac[clusterId] = xpoldetector::kThresholdDacMin;
    *xpollog::kWarning << "Threshold out of range." << endline;
  } else {
    m_thresholdDac[clusterId] = dacSetting;
  }
}
void pDetectorConfiguration::setCalibrationDac(unsigned short int dacSetting)
{
  if (dacSetting > xpoldetector::kCalibrationDacMax){
    m_calibrationDac = xpoldetector::kCalibrationDacMax;
  } else if (dacSetting < xpoldetector::kCalibrationDacMin){
    m_calibrationDac = xpoldetector::kCalibrationDacMin;
  } else {
    m_calibrationDac = dacSetting;
  }
}

int pDetectorConfiguration::getMaxBufferSize()
{
  if(m_bufferMode == xpoldetector::kSmallBufferMode)
    {
      return xpoldetector::kSmallBufferSize;
    } else {
      return xpoldetector::kLargeBufferSize;
    }
}

void pDetectorConfiguration::writeToFile(std::string filePath)
{
  *xpollog::kInfo << "Writing configuration to " << filePath <<
    "... " << endline;
  std::ofstream *outputFile = xpolio::kIOManager->openOutputFile(filePath);
  xpolio::kIOManager->write(outputFile, "//readout_mode//");
  xpolio::kIOManager->write(outputFile, getReadoutMode());
  xpolio::kIOManager->write(outputFile, "//buffer_mode//");
  xpolio::kIOManager->write(outputFile, getBufferMode());
  xpolio::kIOManager->write(outputFile, "//calibration_dac//");
  xpolio::kIOManager->write(outputFile, getCalibrationDac());
  xpolio::kIOManager->write(outputFile, "//pixel_address_x//");
  xpolio::kIOManager->write(outputFile, getPixelAddressX());
  xpolio::kIOManager->write(outputFile, "//pixel_address_y//");
  xpolio::kIOManager->write(outputFile, getPixelAddressY());
  xpolio::kIOManager->write(outputFile, "//threshold_dacs//");
  for (int i = 0; i <  NUM_READOUT_CLUSTERS; i++){
    xpolio::kIOManager->write(outputFile, getThresholdDac(i));
  }
  xpolio::kIOManager->write(outputFile, "//timing_code//");
  xpolio::kIOManager->write(outputFile, getTimingCode());
  xpolio::kIOManager->write(outputFile, "//num_ped_samples//");
  xpolio::kIOManager->write(outputFile, getNumPedSamples());
  xpolio::kIOManager->write(outputFile, "//ped_sample_delay//");
  xpolio::kIOManager->write(outputFile, getPedSampleDelay());
  xpolio::kIOManager->write(outputFile, "//trg_enable_delay//");
  xpolio::kIOManager->write(outputFile, getTrgEnableDelay());
  xpolio::kIOManager->write(outputFile, "//min_window_size//");
  xpolio::kIOManager->write(outputFile, getMinWindowSize());
  xpolio::kIOManager->write(outputFile, "//max_window_size//");
  xpolio::kIOManager->write(outputFile, getMaxWindowSize());
  xpolio::kIOManager->closeOutputFile(outputFile);
}

void pDetectorConfiguration::readFromFile(std::string filePath)
{
  *xpollog::kInfo << "Reading configuration from " << filePath <<
    "... " << endline;
  std::ifstream *inputFile = xpolio::kIOManager->openInputFile(filePath);
  xpolio::kIOManager->skipLine(inputFile);
  setReadoutMode(xpolio::kIOManager->readUnsignedShort(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setBufferMode(xpolio::kIOManager->readUnsignedShort(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setCalibrationDac(xpolio::kIOManager->readUnsignedShort(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setPixelAddressX(xpolio::kIOManager->readUnsignedShort(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setPixelAddressY(xpolio::kIOManager->readUnsignedShort(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  for (int i = 0; i <  NUM_READOUT_CLUSTERS; i++){
    setThresholdDac(i, xpolio::kIOManager->readUnsignedShort(inputFile));
  }
  xpolio::kIOManager->skipLine(inputFile);
  setTimingCode(xpolio::kIOManager->readUnsignedShort(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setNumPedSamples(xpolio::kIOManager->readUnsignedShort(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setPedSampleDelay(xpolio::kIOManager->readUnsignedShort(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setTrgEnableDelay(xpolio::kIOManager->readUnsignedShort(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setMinWindowSize(xpolio::kIOManager->readUnsignedShort(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setMaxWindowSize(xpolio::kIOManager->readUnsignedShort(inputFile));
  xpolio::kIOManager->closeInputFile(inputFile);
}
