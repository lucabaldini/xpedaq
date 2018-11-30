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

#include "xpoldetector.h"

const unsigned short xpoldetector::kUndefinedReadoutCode = 0x0;
const unsigned short xpoldetector::kFullFrameReadoutCode = 0x1;
const unsigned short xpoldetector::kWindowedReadoutCode = 0x2;
const unsigned short xpoldetector::kChargeInjectionReadoutCode = 0x4;
const unsigned short xpoldetector::kThresholdDacNumBits = 12;
const unsigned short xpoldetector::kThresholdDacMin = 0;
const unsigned short xpoldetector::kThresholdDacMax =
(unsigned short)(pow(2, xpoldetector::kThresholdDacNumBits) - 1);
const int xpoldetector::kThresholdVoltageMin = 0;
const int xpoldetector::kThresholdVoltageMax =
xpoldetector::thrDacToVoltage(xpoldetector::kThresholdDacMax - 1);
const int xpoldetector::kThresholdChargeMin = 0;
const int xpoldetector::kThresholdChargeMax =
xpoldetector::thrDacToCharge(xpoldetector::kThresholdDacMax - 1);
const unsigned short xpoldetector::kCalibrationDacNumBits = 12;
const unsigned short xpoldetector::kCalibrationDacMin = 0;
const unsigned short xpoldetector::kCalibrationDacMax =
(unsigned short)(pow(2, xpoldetector::kCalibrationDacNumBits) - 1);
const int xpoldetector::kCalibrationVoltageMin = 0;
const int xpoldetector::kCalibrationVoltageMax =
xpoldetector::calDacToVoltage(xpoldetector::kCalibrationDacMax);
const int xpoldetector::kCalibrationChargeMin = 0;
const int xpoldetector::kCalibrationChargeMax =
xpoldetector::calDacToCharge(xpoldetector::kCalibrationDacMax);
const double xpoldetector::kThresholdDacTomV = 0.806;
const double xpoldetector::kCalibrationDacTomV = 0.806;
const double xpoldetector::kReferenceDacTomV = 0.763;
const double xpoldetector::kThresholdSensitivity = 200.0;
const double xpoldetector::kCalibrationSensitivity = 0.010;
const double xpoldetector::kElectronsPerfC = 6200.0;
const double xpoldetector::kColPitch = 0.0500; //[mm]
const double xpoldetector::kRowPitch = 0.0433; //[mm]

int xpoldetector::thrDacToVoltage(unsigned short dac)
{
  double voltage = dac*kThresholdDacTomV;
  return (int)(0.5 + voltage);
}

int xpoldetector::thrDacToCharge(unsigned short dac)
{
  double charge = dac*kThresholdDacTomV*kElectronsPerfC/kThresholdSensitivity;
  return (int)(0.5 + charge);
}

unsigned short xpoldetector::thrVoltageToDac(int voltage)
{
  double dac = voltage/kThresholdDacTomV;
  return (unsigned short)(0.5 + dac);
}

unsigned short xpoldetector::thrChargeToDac(int charge)
{
  double dac = charge*kThresholdSensitivity/kThresholdDacTomV/kElectronsPerfC;
  return (unsigned short)(0.5 + dac);
}

int xpoldetector::refDacToVoltage(unsigned short dac)
{
  double voltage = dac*kReferenceDacTomV;
  return (int)(0.5 + voltage);
}

int xpoldetector::refDacToCharge(unsigned short dac)
{
  double charge = dac*kReferenceDacTomV*kElectronsPerfC/kThresholdSensitivity;
  return (int)(0.5 + charge);
}

unsigned short xpoldetector::refVoltageToDac(int voltage)
{
  double dac = voltage/kReferenceDacTomV;
  return (unsigned short)(0.5 + dac);
}

unsigned short xpoldetector::refChargeToDac(int charge)
{
  double dac = charge*kThresholdSensitivity/kReferenceDacTomV/kElectronsPerfC;
  return (unsigned short)(0.5 + dac);
}

int xpoldetector::calDacToVoltage(unsigned short dac)
{
  double voltage = dac*kCalibrationDacTomV;
  return (int)(0.5 + voltage);
}

int xpoldetector::calDacToCharge(unsigned short dac)
{
  double charge = dac*kCalibrationDacTomV*kElectronsPerfC*
    kCalibrationSensitivity;
  return (int)(0.5 + charge);
}

unsigned short xpoldetector::calChargeToDac(int charge)
{
  double dac = charge/(kCalibrationSensitivity*kCalibrationDacTomV*
		       kElectronsPerfC);
  return (unsigned short)(0.5 + dac);
}

unsigned short xpoldetector::calVoltageToDac(int voltage)
{
  double dac = voltage/kCalibrationDacTomV;
  return (unsigned short)(0.5 + dac);
}

const unsigned short xpoldetector::kClockShiftMin = 0;
const unsigned short xpoldetector::kClockShiftMax = 800;
const unsigned short xpoldetector::kClockShiftStep = 25;
const unsigned short xpoldetector::kSubSamplesZero = 0;
const unsigned short xpoldetector::kSubSamplesSmaller = 1;
const unsigned short xpoldetector::kSubSamplesSmall = 2;
const unsigned short xpoldetector::kSubSamplesLarge = 4;
const unsigned short xpoldetector::kSubSamplesLarger = 8;
const unsigned short xpoldetector::kPedSubDelayMin = 83;
const unsigned short xpoldetector::kPedSubDelayMax = 833;
const unsigned short xpoldetector::kPedSubDelayStep = 42;
const unsigned short xpoldetector::kTrgEnableDelayMin = 42;
const unsigned short xpoldetector::kTrgEnableDelayMax = 416;
const unsigned short xpoldetector::kTrgEnableDelayStep = 42;
const unsigned short xpoldetector::kMinWindowSizeMin = 32;
const unsigned short xpoldetector::kMinWindowSizeMax = 1024;
const unsigned short xpoldetector::kMinWindowSizeStep = 32;
const unsigned short xpoldetector::kMaxWindowSizeMin = 512;
const unsigned short xpoldetector::kMaxWindowSizeMax = 5120;
const unsigned short xpoldetector::kMaxWindowSizeStep = 512;
const unsigned short xpoldetector::kClockFasterFreqCode = 5;
const unsigned short xpoldetector::kClockFastFreqCode = 10;
const unsigned short xpoldetector::kClockSlowFreqCode = 20;
const unsigned short xpoldetector::kClockSlowerFreqCode = 40;
const unsigned short xpoldetector::kNumPixelsX = 300;
const unsigned short xpoldetector::kNumPixelsY = 352;
const unsigned int xpoldetector::kNumPixels = xpoldetector::kNumPixelsX *
                                              xpoldetector::kNumPixelsY;
const unsigned short xpoldetector::kSmallBufferMode = 0x1;
const unsigned short xpoldetector::kLargeBufferMode = ~xpoldetector::kSmallBufferMode;
const unsigned short xpoldetector::kNumReadOutBuffers = 8;
const unsigned short xpoldetector::kNumThresholdClusters = 16;

const int xpoldetector::kSmallBufferSize = 2*5000;
const int xpoldetector::kLargeBufferSize = 2*262144;
