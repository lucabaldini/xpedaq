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

#ifndef PDETECTORCONFIGURATION_H
#define PDETECTORCONFIGURATION_H

#include "xpedaqutils.h"
#include "xpoldetector.h"
#include "xpolio.h"
#include "xpollog.h"
#include <cstdlib>

class pDetectorConfiguration
{

 public:
  
  pDetectorConfiguration();
  pDetectorConfiguration(std::string filePath);
  ~pDetectorConfiguration() {;}

  // Access functions.
  unsigned short readoutMode() const {return m_readoutMode;}
  unsigned short *thresholdDac() {return m_thresholdDac;}
  unsigned short thresholdDac(int clusterId) const
  {return m_thresholdDac[clusterId];}
  unsigned short calibrationDac() const {return m_calibrationDac;}
  unsigned short pixelAddressX() const {return m_pixelAddressX;}
  unsigned short pixelAddressY() const {return m_pixelAddressY;}
  unsigned short clockFrequency() const {return m_clockFrequency;}
  unsigned short clockShift() const {return m_clockShift;}
  unsigned short timingCode() const;
  unsigned short numPedSamples() const {return m_numPedSamples;}
  unsigned short pedSampleDelay() const {return m_pedSampleDelay;}
  unsigned short trgEnableDelay() const {return m_trgEnableDelay;}
  unsigned short minWindowSize() const {return m_minWindowSize;}
  unsigned short maxWindowSize() const {return m_maxWindowSize;}
  bool windowMarginHigh() const {return m_windowMarginHigh;}
  unsigned short probeAddressA() const {return m_probeAddressA;}
  unsigned short probeAddressB() const {return m_probeAddressB;}
  unsigned short probeAddressC() const {return m_probeAddressC;}
  unsigned short bufferMode() const {return m_bufferMode;}
  unsigned short adcTestPattern() {return m_adcTestPattern;}
  int maxBufferSize() const;

  // Set functions.
  void setReadoutMode(unsigned short mode);
  void setThresholdDac(int clusterId, unsigned short dacSetting);
  void setThresholdDac(unsigned short dacSetting);
  void setCalibrationDac(unsigned short dacSetting);
  void setPixelAddressX(unsigned short address) {m_pixelAddressX = address;}
  void setPixelAddressY(unsigned short address) {m_pixelAddressY = address;}
  void setClockFrequency(unsigned short freq) {m_clockFrequency = freq;}
  void setClockShift(unsigned short shift) {m_clockShift = shift;}
  void setNumPedSamples(unsigned short samples) {m_numPedSamples = samples;}
  void setPedSampleDelay(unsigned short delay) {m_pedSampleDelay = delay;}
  void setTrgEnableDelay(unsigned short delay) {m_trgEnableDelay = delay;}
  void setMinWindowSize(unsigned short size) {m_minWindowSize = size;}
  void setMaxWindowSize(unsigned short size) {m_maxWindowSize = size;}
  void setWindowMarginHigh(bool high) {m_windowMarginHigh = high;}
  void setProbeAddressA(unsigned short address) {m_probeAddressA = address;}
  void setProbeAddressB(unsigned short address) {m_probeAddressB = address;}
  void setProbeAddressC(unsigned short address) {m_probeAddressC = address;}
  void setBufferMode(unsigned short mode) {m_bufferMode = mode;}
  void setAdcTestPattern(unsigned short pattern) {m_adcTestPattern = pattern;}

  ///\brief Write the configuration to file.
  void writeToFile(std::string filePath);

  ///\brief Read the configuration from file.
  void readFromFile(std::string filePath);

  // Terminal formatting.
  std::ostream& fillStream(std::ostream& os) const;
  friend std::ostream& operator<<(std::ostream& os,
				  const pDetectorConfiguration& configuration)
  {return configuration.fillStream(os);}

 protected:

 private:
  
  unsigned short m_readoutMode;
  unsigned short m_thresholdDac[NUM_READOUT_CLUSTERS];
  unsigned short m_calibrationDac;
  unsigned short m_pixelAddressX;
  unsigned short m_pixelAddressY;
  unsigned short m_clockFrequency;
  unsigned short m_clockShift;
  unsigned short m_numPedSamples;
  unsigned short m_pedSampleDelay;
  unsigned short m_trgEnableDelay;
  unsigned short m_minWindowSize;
  unsigned short m_maxWindowSize;
  bool m_windowMarginHigh;
  unsigned short m_probeAddressA;
  unsigned short m_probeAddressB;
  unsigned short m_probeAddressC;
  unsigned short m_adcTestPattern;
  unsigned short m_bufferMode;
};

#endif //PDETECTORCONFIGURATION_H
