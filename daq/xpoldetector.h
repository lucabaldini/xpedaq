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

#define NUM_READOUT_CLUSTERS 16

#ifndef XPOLDETECTOR_H
#define XPOLDETECTOR_H

#include <math.h>
#include <vector>
#include <cstdint>

typedef uint16_t adc_count_t;
typedef uint64_t microsecond_t;

namespace event
{
  typedef std::vector<adc_count_t> Adc_vec_t;
 
  struct Hit{
    double x;
    double y;
    adc_count_t counts;
    int clusterId;
  };
}

namespace xpoldetector{
  extern const unsigned short kUndefinedReadoutCode;
  extern const unsigned short kFullFrameReadoutCode;
  extern const unsigned short kWindowedReadoutCode;
  extern const unsigned short kChargeInjectionReadoutCode;
  extern const unsigned short kThresholdDacNumBits;
  extern const unsigned short kThresholdDacMin;
  extern const unsigned short kThresholdDacMax;
  extern const int kThresholdVoltageMin;
  extern const int kThresholdVoltageMax;
  extern const int kThresholdChargeMin;
  extern const int kThresholdChargeMax;
  extern const unsigned short kCalibrationDacNumBits;
  extern const unsigned short kCalibrationDacMin;
  extern const unsigned short kCalibrationDacMax;
  extern const int kCalibrationVoltageMin;
  extern const int kCalibrationVoltageMax;
  extern const int kCalibrationChargeMin;
  extern const int kCalibrationChargeMax;
  extern const double kThresholdDacTomV;
  extern const double kCalibrationDacTomV;
  extern const double kReferenceDacTomV;
  extern const double kThresholdSensitivity;
  extern const double kCalibrationSensitivity;
  extern const double kElectronsPerfC;
  int thrDacToVoltage(unsigned short dac);
  int thrDacToCharge(unsigned short dac);
  unsigned short thrVoltageToDac(int voltage);
  unsigned short thrChargeToDac(int charge);
  int refDacToVoltage(unsigned short dac);
  int refDacToCharge(unsigned short dac);
  unsigned short refVoltageToDac(int voltage);
  unsigned short refChargeToDac(int charge);
  int calDacToVoltage(unsigned short dac);
  int calDacToCharge(unsigned short dac);
  unsigned short calVoltageToDac(int voltage);
  unsigned short calChargeToDac(int charge);
  extern const unsigned short kClockShiftMin;
  extern const unsigned short kClockShiftMax;
  extern const unsigned short kClockShiftStep;
  extern const unsigned short kSubSamplesZero;
  extern const unsigned short kSubSamplesSmaller;
  extern const unsigned short kSubSamplesSmall;
  extern const unsigned short kSubSamplesLarge;
  extern const unsigned short kSubSamplesLarger;
  extern const unsigned short kPedSubDelayMin;
  extern const unsigned short kPedSubDelayMax;
  extern const unsigned short kPedSubDelayStep;
  extern const unsigned short kTrgEnableDelayMin;
  extern const unsigned short kTrgEnableDelayMax;
  extern const unsigned short kTrgEnableDelayStep;
  extern const unsigned short kMinWindowSizeMin;
  extern const unsigned short kMinWindowSizeMax;
  extern const unsigned short kMinWindowSizeStep;
  extern const unsigned short kMaxWindowSizeMin;
  extern const unsigned short kMaxWindowSizeMax;
  extern const unsigned short kMaxWindowSizeStep;
  extern const unsigned short kClockFasterFreqCode;
  extern const unsigned short kClockFastFreqCode;
  extern const unsigned short kClockSlowFreqCode;
  extern const unsigned short kClockSlowerFreqCode;
  extern const unsigned short kNumPixelsX;
  extern const unsigned short kNumPixelsY;
  extern const unsigned int kNumPixels;
  extern const unsigned short kLargeBufferMode;
  extern const unsigned short kSmallBufferMode;
  extern const unsigned short kNumReadOutBuffers;
  extern const unsigned short kNumThresholdClusters;
  extern const int kSmallBufferSize;
  extern const int kLargeBufferSize;
  extern const double kColPitch;
  extern const double kRowPitch;
}

#endif
