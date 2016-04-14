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

#ifndef PDETECTORCONFIGURATION_H
#define PDETECTORCONFIGURATION_H

//#include "pUsbController.h"  //cannot do that QThreads complains
#include "xpoldetector.h"
#include "xpolio.h"
#include "xpollog.h"
#include <cstdlib>

class pDetectorConfiguration
{

 public:
  pDetectorConfiguration();
  ~pDetectorConfiguration() {;}
  inline unsigned short int getReadoutMode()
    {return m_readoutMode;}
  void setReadoutMode(unsigned short int mode);
  inline unsigned short int *getThresholdDac()
    {return m_thresholdDac;}
  inline unsigned short int getThresholdDac(int clusterId)
    {return m_thresholdDac[clusterId];}
  void setThresholdDac(int clusterId, unsigned short int dacSetting);
  unsigned short int getCalibrationDac()
    {return m_calibrationDac;}
  void setCalibrationDac(unsigned short int dacSetting);
  inline unsigned short int getPixelAddressX()
    {return m_pixelAddressX;}
  void setPixelAddressX(unsigned short int address)
    {m_pixelAddressX = address;}
  inline unsigned short int getPixelAddressY()
    {return m_pixelAddressY;}
  void setPixelAddressY(unsigned short int address)
    {m_pixelAddressY = address;}
  inline unsigned char getTimingCode()
    {return m_timingCode;}
  void setTimingCode(unsigned char code)
    {m_timingCode = code;}
  inline unsigned short int getNumPedSamples()
    {return m_numPedSamples;}
  void setNumPedSamples(unsigned short int samples)
    {m_numPedSamples = samples;}
  inline unsigned short int getPedSampleDelay()
    {return m_pedSampleDelay;}
  void setPedSampleDelay(unsigned short int delay)
    {m_pedSampleDelay = delay;}
  inline unsigned short int getTrgEnableDelay()
    {return m_trgEnableDelay;}
  void setTrgEnableDelay(unsigned short int delay)
    {m_trgEnableDelay = delay;}
  inline unsigned short int getMinWindowSize()
    {return m_minWindowSize;}
  void setMinWindowSize(unsigned short int size)
    {m_minWindowSize = size;}
  inline unsigned short int getMaxWindowSize()
    {return m_maxWindowSize;}
  void setMaxWindowSize(unsigned short int size)
    {m_maxWindowSize = size;}
  inline unsigned short int getProbeAddressA()
    {return m_probeAddressA;}
  void setProbeAddressA(unsigned short int address)
    {m_probeAddressA = address;}
  inline unsigned short int getProbeAddressB()
    {return m_probeAddressB;}
  void setProbeAddressB(unsigned short int address)
    {m_probeAddressB = address;}
  inline unsigned short int getProbeAddressC()
    {return m_probeAddressC;}
  void setProbeAddressC(unsigned short int address)
    {m_probeAddressC = address;}
  inline unsigned short int getBufferMode()
    {return m_bufferMode;}
  void setBufferMode(unsigned short int mode)
    {m_bufferMode = mode;}
  int getMaxBufferSize();
  inline unsigned short int adcTestPattern()
    {return m_adcTestPattern;}
  void setAdcTestPattern(unsigned short int pattern)
    {m_adcTestPattern = pattern;}
  void writeToFile(std::string filePath);
  void readFromFile(std::string filePath);

 protected:

 private:
  unsigned short int m_readoutMode;
  unsigned short int m_thresholdDac[NUM_READOUT_CLUSTERS];
  unsigned short int m_calibrationDac;
  unsigned short int m_pixelAddressX;
  unsigned short int m_pixelAddressY;
  unsigned char m_timingCode;
  unsigned short int m_numPedSamples;
  unsigned short int m_pedSampleDelay;
  unsigned short int m_trgEnableDelay;
  unsigned short int m_minWindowSize;
  unsigned short int m_maxWindowSize;
  unsigned short int m_probeAddressA;
  unsigned short int m_probeAddressB;
  unsigned short int m_probeAddressC;
  unsigned short int m_adcTestPattern;
  unsigned short int m_bufferMode;
};

#endif
