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

#include "pDetectorConfiguration.h"


/*!
 */
pDetectorConfiguration::pDetectorConfiguration()
{

}


/*!
 */
pDetectorConfiguration::pDetectorConfiguration(std::string filePath)
{
  readFromFile(filePath);
}


/*!
 */
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


/*!
 */
void pDetectorConfiguration::setThresholdDac(int clusterId,
					     unsigned short int dacSetting)
{
  if (dacSetting > xpoldetector::kThresholdDacMax) {
    m_thresholdDac[clusterId] = xpoldetector::kThresholdDacMax;
    *xpollog::kWarning << "Threshold out of range." << endline;
  } else if (dacSetting < xpoldetector::kThresholdDacMin) {
    m_thresholdDac[clusterId] = xpoldetector::kThresholdDacMin;
    *xpollog::kWarning << "Threshold out of range." << endline;
  } else {
    m_thresholdDac[clusterId] = dacSetting;
  }
}


/*!
 */
void pDetectorConfiguration::setCalibrationDac(unsigned short int dacSetting)
{
  if (dacSetting > xpoldetector::kCalibrationDacMax) {
    m_calibrationDac = xpoldetector::kCalibrationDacMax;
  } else if (dacSetting < xpoldetector::kCalibrationDacMin) {
    m_calibrationDac = xpoldetector::kCalibrationDacMin;
  } else {
    m_calibrationDac = dacSetting;
  }
}


/*!
 */
unsigned char pDetectorConfiguration::timingCode() const
{
  return m_clockFrequency | (m_clockShift & 0x1f);
}


/*!
 */
int pDetectorConfiguration::maxBufferSize() const
{
  if (m_bufferMode == xpoldetector::kSmallBufferMode) {
    return xpoldetector::kSmallBufferSize;
  } else {
    return xpoldetector::kLargeBufferSize;
  }
}


/*!
 */
void pDetectorConfiguration::writeToFile(std::string filePath)
{
  *xpollog::kInfo << "Writing configuration to " << filePath <<
    "... " << endline;
  std::ofstream *outputFile = xpolio::kIOManager->openOutputFile(filePath);
  xpolio::kIOManager->write(outputFile, "//Readout mode//");
  xpolio::kIOManager->write(outputFile, readoutMode());
  xpolio::kIOManager->write(outputFile, "//Buffer mode//");
  xpolio::kIOManager->write(outputFile, bufferMode());
  xpolio::kIOManager->write(outputFile, "//Calibration dac//");
  xpolio::kIOManager->write(outputFile, calibrationDac());
  xpolio::kIOManager->write(outputFile, "//Pixel address x//");
  xpolio::kIOManager->write(outputFile, pixelAddressX());
  xpolio::kIOManager->write(outputFile, "//Pixel address y//");
  xpolio::kIOManager->write(outputFile, pixelAddressY());
  xpolio::kIOManager->write(outputFile, "//Threshold dacs//");
  for (int i = 0; i <  NUM_READOUT_CLUSTERS; i++) {
    xpolio::kIOManager->write(outputFile, thresholdDac(i));
  }
  //xpolio::kIOManager->write(outputFile, "//Timing code//");
  //xpolio::kIOManager->write(outputFile, timingCode());
  xpolio::kIOManager->write(outputFile, "//Clock frequency code//");
  xpolio::kIOManager->write(outputFile, clockFrequency());
  xpolio::kIOManager->write(outputFile, "//Clock shift//");
  xpolio::kIOManager->write(outputFile, clockShift());
  xpolio::kIOManager->write(outputFile, "//# pedestal samples//");
  xpolio::kIOManager->write(outputFile, numPedSamples());
  xpolio::kIOManager->write(outputFile, "//Pedestal sample delay//");
  xpolio::kIOManager->write(outputFile, pedSampleDelay());
  xpolio::kIOManager->write(outputFile, "//Trigger enable delay//");
  xpolio::kIOManager->write(outputFile, trgEnableDelay());
  xpolio::kIOManager->write(outputFile, "//Minimum window size//");
  xpolio::kIOManager->write(outputFile, minWindowSize());
  xpolio::kIOManager->write(outputFile, "//Maximum window size//");
  xpolio::kIOManager->write(outputFile, maxWindowSize());
  xpolio::kIOManager->closeOutputFile(outputFile);
}


/*!
 */
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
  for (int i = 0; i <  NUM_READOUT_CLUSTERS; i++) {
    setThresholdDac(i, xpolio::kIOManager->readUnsignedShort(inputFile));
  }
  xpolio::kIOManager->skipLine(inputFile);
  //  setTimingCode(xpolio::kIOManager->readUnsignedShort(inputFile));
  setClockFrequency(xpolio::kIOManager->readUnsignedShort(inputFile));
  xpolio::kIOManager->skipLine(inputFile);
  setClockShift(xpolio::kIOManager->readUnsignedShort(inputFile));
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


/*!
 */
std::ostream& pDetectorConfiguration::fillStream(std::ostream& os) const
{
  os << xpedaqutils::title("Detector configuration", true);
  os << "Readout mode: " << readoutMode() << std::endl;
  os << "Buffer mode: " << bufferMode() << std::endl;
  os << "Calibration DAC: " << calibrationDac() << std::endl;
  os << "Pixel address x: " << pixelAddressX() << std::endl;
  os << "Pixel address y: " << pixelAddressY() << std::endl;
  os << "Threshold DACs: ";
  for (int i = 0; i <  NUM_READOUT_CLUSTERS; i++) {
    os << thresholdDac(i) << " ";
  }
  os << std::endl;
  //os << "Timing code: " << static_cast<unsigned short> (timingCode())
  //   << std::endl;
  os << "Clock frequency code " << clockFrequency() << std::endl;
  os << "Clock shift " << clockShift() << std::endl;
  os << "# samples for pedestals: " << numPedSamples() << std::endl;
  os << "Pedestal sample delay: " << pedSampleDelay() << std::endl;
  os << "Trigger enable delay: " << trgEnableDelay() << std::endl;
  os << "Minimum window size: " << minWindowSize() << std::endl;
  os << "Maximum window size: " << maxWindowSize() << std::endl;
  os << xpedaqutils::hline();
  return os;
}
