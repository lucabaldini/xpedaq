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

#define NUM_READOUT_CLUSTERS 16

#ifndef XPOLDETECTOR_H
#define XPOLDETECTOR_H

#include <math.h>
#include <iostream>

namespace xpoldetector{
  extern const short unsigned int kUndefinedReadoutCode;
  extern const short unsigned int kFullFrameReadoutCode;
  extern const short unsigned int kWindowedReadoutCode;
  extern const short unsigned int kChargeInjectionReadoutCode;
  extern const short unsigned int kThresholdDacNumBits;
  extern const short unsigned int kThresholdDacMin;
  extern const short unsigned int kThresholdDacMax;
  extern const int kThresholdVoltageMin;
  extern const int kThresholdVoltageMax;
  extern const int kThresholdChargeMin;
  extern const int kThresholdChargeMax;
  extern const short unsigned int kCalibrationDacNumBits;
  extern const short unsigned int kCalibrationDacMin;
  extern const short unsigned int kCalibrationDacMax;
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
  int thrDacToVoltage(short unsigned int dac);
  int thrDacToCharge(short unsigned int dac);
  short unsigned int thrVoltageToDac(int voltage);
  short unsigned int thrChargeToDac(int charge);
  int refDacToVoltage(short unsigned int dac);
  int refDacToCharge(short unsigned int dac);
  short unsigned int refVoltageToDac(int voltage);
  short unsigned int refChargeToDac(int charge);
  int calDacToVoltage(short unsigned int dac);
  int calDacToCharge(short unsigned int dac);
  short unsigned int calVoltageToDac(int voltage);
  short unsigned int calChargeToDac(int charge);
  extern unsigned short int kClockShiftMin;
  extern unsigned short int kClockShiftMax;
  extern unsigned short int kClockShiftStep;
  extern unsigned short int kSubSamplesZero;
  extern unsigned short int kSubSamplesSmaller;
  extern unsigned short int kSubSamplesSmall;
  extern unsigned short int kSubSamplesLarge;
  extern unsigned short int kSubSamplesLarger;
  extern unsigned short int kPedSubDelayMin;
  extern unsigned short int kPedSubDelayMax;
  extern unsigned short int kPedSubDelayStep;
  extern unsigned short int kTrgEnableDelayMin;
  extern unsigned short int kTrgEnableDelayMax;
  extern unsigned short int kTrgEnableDelayStep;
  extern unsigned short int kMinWindowSizeMin;
  extern unsigned short int kMinWindowSizeMax;
  extern unsigned short int kMinWindowSizeStep;
  extern unsigned short int kMaxWindowSizeMin;
  extern unsigned short int kMaxWindowSizeMax;
  extern unsigned short int kMaxWindowSizeStep;
  extern unsigned short int kClockFasterFreqCode;
  extern unsigned short int kClockFastFreqCode;
  extern unsigned short int kClockSlowFreqCode;
  extern unsigned short int kClockSlowerFreqCode;
  extern unsigned short int kNumPixelsX;
  extern unsigned short int kNumPixelsY;
  extern unsigned short int kLargeBufferMode;
  extern unsigned short int kSmallBufferMode;
  extern int kSmallBufferSize;
  extern int kLargeBufferSize;
}

#endif
