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

#include "xpoldetector.h"

const short unsigned int xpoldetector::kUndefinedReadoutCode = 0x0;
const short unsigned int xpoldetector::kFullFrameReadoutCode = 0x1;
const short unsigned int xpoldetector::kWindowedReadoutCode = 0x2;
const short unsigned int xpoldetector::kChargeInjectionReadoutCode = 0x4;
const short unsigned int xpoldetector::kThresholdDacNumBits = 12;
const short unsigned int xpoldetector::kThresholdDacMin = 0;
const short unsigned int xpoldetector::kThresholdDacMax =
(short unsigned int)(pow(2, xpoldetector::kThresholdDacNumBits) - 1);
const int xpoldetector::kThresholdVoltageMin = 0;
const int xpoldetector::kThresholdVoltageMax =
xpoldetector::thrDacToVoltage(xpoldetector::kThresholdDacMax - 1);
const int xpoldetector::kThresholdChargeMin = 0;
const int xpoldetector::kThresholdChargeMax =
xpoldetector::thrDacToCharge(xpoldetector::kThresholdDacMax - 1);
const short unsigned int xpoldetector::kCalibrationDacNumBits = 12;
const short unsigned int xpoldetector::kCalibrationDacMin = 0;
const short unsigned int xpoldetector::kCalibrationDacMax =
(short unsigned int)(pow(2, xpoldetector::kCalibrationDacNumBits) - 1);
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

int xpoldetector::thrDacToVoltage(short unsigned int dac)
{
  double voltage = dac*kThresholdDacTomV;
  return (int)(0.5 + voltage);
}

int xpoldetector::thrDacToCharge(short unsigned int dac)
{
  double charge = dac*kThresholdDacTomV*kElectronsPerfC/kThresholdSensitivity;
  return (int)(0.5 + charge);
}

short unsigned int xpoldetector::thrVoltageToDac(int voltage)
{
  double dac = voltage/kThresholdDacTomV;
  return (short unsigned int)(0.5 + dac);
}

short unsigned int xpoldetector::thrChargeToDac(int charge)
{
  double dac = charge*kThresholdSensitivity/kThresholdDacTomV/kElectronsPerfC;
  return (short unsigned int)(0.5 + dac);
}

int xpoldetector::refDacToVoltage(short unsigned int dac)
{
  double voltage = dac*kReferenceDacTomV;
  return (int)(0.5 + voltage);
}

int xpoldetector::refDacToCharge(short unsigned int dac)
{
  double charge = dac*kReferenceDacTomV*kElectronsPerfC/kThresholdSensitivity;
  return (int)(0.5 + charge);
}

short unsigned int xpoldetector::refVoltageToDac(int voltage)
{
  double dac = voltage/kReferenceDacTomV;
  return (short unsigned int)(0.5 + dac);
}

short unsigned int xpoldetector::refChargeToDac(int charge)
{
  double dac = charge*kThresholdSensitivity/kReferenceDacTomV/kElectronsPerfC;
  return (short unsigned int)(0.5 + dac);
}

int xpoldetector::calDacToVoltage(short unsigned int dac)
{
  double voltage = dac*kCalibrationDacTomV;
  return (int)(0.5 + voltage);
}

int xpoldetector::calDacToCharge(short unsigned int dac)
{
  double charge = dac*kCalibrationDacTomV*kElectronsPerfC*
    kCalibrationSensitivity;
  return (int)(0.5 + charge);
}

short unsigned int xpoldetector::calChargeToDac(int charge)
{
  double dac = charge/(kCalibrationSensitivity*kCalibrationDacTomV*
		       kElectronsPerfC);
  return (short unsigned int)(0.5 + dac);
}

short unsigned int xpoldetector::calVoltageToDac(int voltage)
{
  double dac = voltage/kCalibrationDacTomV;
  return (short unsigned int)(0.5 + dac);
}

unsigned short int xpoldetector::kClockShiftMin = 0;
unsigned short int xpoldetector::kClockShiftMax = 800;
unsigned short int xpoldetector::kClockShiftStep = 25;
unsigned short int xpoldetector::kSubSamplesZero = 0;
unsigned short int xpoldetector::kSubSamplesSmaller = 1;
unsigned short int xpoldetector::kSubSamplesSmall = 2;
unsigned short int xpoldetector::kSubSamplesLarge = 4;
unsigned short int xpoldetector::kSubSamplesLarger = 8;
unsigned short int xpoldetector::kPedSubDelayMin = 100;
unsigned short int xpoldetector::kPedSubDelayMax = 1000;
unsigned short int xpoldetector::kPedSubDelayStep = 50;
unsigned short int xpoldetector::kTrgEnableDelayMin = 50;
unsigned short int xpoldetector::kTrgEnableDelayMax = 500;
unsigned short int xpoldetector::kTrgEnableDelayStep = 50;
unsigned short int xpoldetector::kMinWindowSizeMin = 32;
unsigned short int xpoldetector::kMinWindowSizeMax = 1024;
unsigned short int xpoldetector::kMinWindowSizeStep = 32;
unsigned short int xpoldetector::kMaxWindowSizeMin = 512;
unsigned short int xpoldetector::kMaxWindowSizeMax = 5120;
unsigned short int xpoldetector::kMaxWindowSizeStep = 512;
unsigned short int xpoldetector::kClockFasterFreqCode = 0x0;
unsigned short int xpoldetector::kClockFastFreqCode = 0x20;
unsigned short int xpoldetector::kClockSlowFreqCode = 0x40;
unsigned short int xpoldetector::kClockSlowerFreqCode = 0x60;
unsigned short int xpoldetector::kNumPixelsX = 352;
unsigned short int xpoldetector::kNumPixelsY = 300;
unsigned short int xpoldetector::kSmallBufferMode = 0x1;
unsigned short int xpoldetector::kLargeBufferMode = ~xpoldetector::kSmallBufferMode;

int xpoldetector::kSmallBufferSize = 10000;
int xpoldetector::kLargeBufferSize = 2*262144;
