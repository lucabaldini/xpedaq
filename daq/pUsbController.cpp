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

#include "pUsbController.h"


/*! Do-nothing constructor.
 */
pUsbController::pUsbController()
{

}


/*! Connect to the QuickUsb interface.
 */
unsigned long pUsbController::connect()
{
  *xpollog::kInfo << "Searching for USB module(s)... " << endline;
  unsigned long errorCode = 0;
  char moduleString[512];
  if (QuickUsbFindModules(moduleString, 512)) {
    int numUsbDevices = 0;
    char devName[20];
    char *moduleStringPtr = moduleString;
    while (*moduleStringPtr != '\0') {
      *xpollog::kDebug << "Found " << moduleStringPtr << "." << endline;
      strncpy(devName, moduleStringPtr, 20);
      moduleStringPtr += strlen(moduleStringPtr);
      numUsbDevices ++;
    }
    if (numUsbDevices > 1) {
      *xpollog::kWarning << numUsbDevices << " modules found, using "
			 << devName << "..." << endline;
    }
    SetName(devName);
    if (!Open()) {
      *xpollog::kError << "Could not open the USB device." << endline;
      exit(1);
    }
    *xpollog::kInfo << "Device name " << deviceName() << "." << endline;
    *xpollog::kInfo << "Driver version " << driverVersion() << "." << endline;
    *xpollog::kInfo << "Dll version " << dllVersion() << "." << endline;
    *xpollog::kInfo << "Firmware version " << firmwareVersion() << "."
		    << endline;
    emit connected(QString::fromStdString(deviceName()),
		   QString::fromStdString(driverVersion()),
		   QString::fromStdString(dllVersion()),
		   QString::fromStdString(firmwareVersion()));
    setup();  
  } else {
    QuickUsbGetLastError(&errorCode);
    if (errorCode > 0) {
      errorSummary(errorCode);
    }
    *xpollog::kError << "Could not connect to USB." << endline;
    emit quickusbError(errorCode);
  }
  return errorCode;
}


/*!
 */
std::string pUsbController::deviceName()
{
  return std::string(GetDeviceName());
}


/*!
 */
std::string pUsbController::driverVersion()
{
  short unsigned int major, minor, build;
  std::stringstream version("");
  GetDriverVersion(&major, &minor, &build);
  version << major << "." << minor << "." << build;
  return version.str();
}
  

/*!
 */
std::string pUsbController::dllVersion()
{
  short unsigned int major, minor, build;
  std::stringstream version("");
  GetDllVersion(&major, &minor, &build);
  version << major << "." << minor << "." << build;
  return version.str();
}


/*!
 */
std::string pUsbController::firmwareVersion()
{
  short unsigned int major, minor, build;
  std::stringstream version("");
  GetFirmwareVersion(&major, &minor, &build);
  version << major << "." << minor << "." << build;
  return version.str();
}


/*!
 */
pUsbController::~pUsbController()
{
  if (!Close()) {
    *xpollog::kError << "Could not Close() the USB device." << endline;
    exit(1);
  }
}


/*!
 */
unsigned long pUsbController::lastErrorCode(bool verbose)
{
  unsigned long errorCode;
  QuickUsbGetLastError(&errorCode);
  if (verbose) {
    errorSummary(errorCode);
  }
  return errorCode;
}


/*!
 */
void pUsbController::errorSummary(unsigned long errorCode)
{
  *xpollog::kError <<  "Error code " << errorCode << ": " <<
    errorDescription(errorCode) << endline;
}


/*!
 */
int pUsbController::readSetting(unsigned short address, unsigned short *value)
{
  *xpollog::kDebug << "Reading address " << address << " (0x" << hex <<
    address << dec << ")..." << flush;
  if (!ReadSetting(address, value))
    {
      *xpollog::kDebug << endline;
      return lastErrorCode();
    }
  *xpollog::kDebug << " got " << *value << " (0x" << hex <<
    *value << dec << ")." << endline;
  return 0;
}


/*!
 */
int pUsbController::readData(unsigned char *data, unsigned long *length)
{
  if (!ReadData(data, length)) {
    return lastErrorCode();
  }
  return 0;
}


/*!
 */
int pUsbController::writeSetting(unsigned short address, unsigned short value)
{
  *xpollog::kDebug << "Writing " << value << " (0x" << hex << value << ")" <<
    dec << " to address " << address << " (0x" << hex << address << dec <<
    ")" << "..." << flush;
  if (!WriteSetting(address, value)) {
    *xpollog::kDebug << endline;
    return lastErrorCode();
  }
  *xpollog::kDebug << " Done." << endline;
  return 0;
}


/*!
 */
int pUsbController::writePortDir(unsigned short address, unsigned char data)
{
  *xpollog::kDebug << "Writing port dir at address " << address << " (0x" <<
    hex << address << dec << ")..." << flush;
  if (!CQuickUsb::WritePortDir(address, data)) {
    *xpollog::kDebug << endline;
    return lastErrorCode();
  }
  *xpollog::kDebug << " Done." << endline;
  return 0;
}


/*!
 */
int pUsbController::writePort(unsigned short address, unsigned char *data,
			      unsigned short length)
{
  *xpollog::kDebug << "Writing port at address " << address << " (0x" <<
    hex << address << dec << ")..." << flush;
  if (!CQuickUsb::WritePort(address, data, length))
    {
      *xpollog::kDebug << endline;
      return lastErrorCode();
    }
  *xpollog::kDebug << " Done." << endline;
  return 0;
}


/*!
 */
int pUsbController::setTimeout(unsigned long timeout)
{
  *xpollog::kDebug << "Setting USB timeout to " << timeout << " ms..."
		   << flush;
  if (!SetTimeout(timeout)) {
    *xpollog::kDebug << endline;
    return lastErrorCode();
  }
  m_timeout = timeout;
  *xpollog::kDebug << " Done." << endline;
  return 0;
}


/*!
 */
int pUsbController::setup()
{
  *xpollog::kInfo << "Setting up the USB controller..." << endline;
  *xpollog::kInfo << "Performing initial configuration check..." << endline;
  if (readUsbSettings()) {
    exit(1);
  }
  *xpollog::kInfo << "Configuring the USB controller..." << endline;
  if (writeUsbSettings()) {
    exit(1);
  }
  *xpollog::kInfo << "Verifying configuration..." << endline;
  if (readUsbSettings()) {
    exit(1);
  }
  setTimeout(TIMEOUTMS);
  stopSequencer();
  flushQUsbFIFO();
  return 1;
}


/*!
 */
int pUsbController::readUsbSettings()
{
  *xpollog::kInfo << "Reading QuickUSB settings..." << endline;
  unsigned long errorCode = 0;
  unsigned short value = 0;
  errorCode += readSetting(SETTING_WORDWIDE   , &value);
  errorCode += readSetting(SETTING_EP26CONFIG , &value);
  errorCode += readSetting(SETTING_DATAADDRESS, &value);
  errorCode += readSetting(SETTING_FIFO_CONFIG, &value);
  errorCode += readSetting(SETTING_PORTACCFG  , &value);
  return errorCode;
}


/*!
 */
int pUsbController::writeUsbSettings()
{
  *xpollog::kInfo << "Writing QuickUSB settings..." << endline;
  unsigned long errorCode = 0;
  unsigned short value = 0;
  errorCode += readSetting(SETTING_WORDWIDE    , &value);
  errorCode += writeSetting(SETTING_WORDWIDE   , value | WORDWIDE_SETT);
  errorCode += writeSetting(SETTING_EP26CONFIG , 0xe2);
  errorCode += readSetting(SETTING_DATAADDRESS , &value);
  errorCode += writeSetting(SETTING_DATAADDRESS, value | DATAADDRESS_SETT);
  errorCode += readSetting(SETTING_FIFO_CONFIG , &value);
  errorCode += writeSetting(SETTING_FIFO_CONFIG, (value | 0x00f3) & 0xffe7);
  errorCode += writePortDir(PORTC, (unsigned char)ALLOUTPUT);
  resetSequencer();
  errorCode += writeSetting(SETTING_PORTACCFG  , 0X4000);
  errorCode += writePortDir(PORTA, (unsigned char)ALLINPUT);
  errorCode += writePortDir(PORTE, (unsigned char)0xbf);
  return errorCode;
}


/*!
 */
int pUsbController::startSequencer()
{
  *xpollog::kInfo << "Starting QuickUsb sequencer..." << endline;
  unsigned char value[1] = {0xc5};
  return writePort(PORTC, value, 1);
}


/*!
 */
int pUsbController::stopSequencer()
{
  *xpollog::kInfo << "Stopping QuickUsb sequencer..." << endline;
  unsigned char value[1] = {0xf4};
  return writePort(PORTC, value, 1);
}


/*!
 */
int pUsbController::resetSequencer()
{
  *xpollog::kInfo << "Resetting QuickUsb sequencer..." << endline;
  unsigned char value[1] = {0xf6};
  return writePort(PORTC, value, 1);
}


/*!
 */
int pUsbController::flushQUsbFIFO()
{
  *xpollog::kInfo << "Flushing QuickUsb FIFO..." << endline;
  unsigned long errorCode = 0;
  unsigned short status = 0;
  long unsigned int length = SRAM_DIM*2;
  unsigned char buffer[SRAM_DIM*2];
  // Keep track of the previou timeout.
  unsigned long prevTimeout = timeout();
  // Set a short timeout.
  errorCode += setTimeout(1000);
  errorCode += readSetting(SETTING_SLAVEFIFOFLAGS, &status);
  if (status & EP6EMPTYFLG) {
    *xpollog::kDebug << "QuickUsb FIFO is empty." << endline;
  } else {
    errorCode += readData((unsigned char*)buffer, &length);
  }
  // Restore the previous timeout.
  errorCode += setTimeout(prevTimeout);
  return errorCode;
}


/*!
 */
std::string pUsbController::errorDescription(unsigned long errorCode)
{
  switch(errorCode)
    {
    case 0:
      return "No error.";
      break;
    case 1:
      return "Out of memory.";
      break;  
    case 2:
      return "Cannot open module.";
      break;
    case 3:
      return "Cannot find specified QuickUsb module.";
      break;  
    case 4:
      return "IOCTL failed (error returned by the driver to the library).";
      break;
    case 5:
      return "Cannot read or write data length of zero or greater than 16 MB.";
      break;  
    case 6:
      return "Timeout occurred while attempting to read or write data.";
      break;
    case 7:
      return "Function not supported by the version of the QuickUsb driver.";
      break;  
    case 8:
      return "I2C bus error.";
      break;
    case 9:
      return "No ACK received from I2C device.";
      break;  
    case 10:
      return "An I2C slave device is holding SCL low.";
      break;
    case 11:
      return "Timeout on the I2C bus.";
      break;
    default:
      return "Unknown error.";
      break;
    }
}


/*!
 */
std::string pUsbController::errorResolution(unsigned long errorCode)
{
  switch(errorCode)
    {
    case 1:
      return "Please free some memory and try again.";
      break;  
    case 3:
      return "Please check the specified module name.";
      break;  
    case 5:
      return "Try breaking the transef up into smaller block.";
      break;
    case 7:
      return "Please update your driver to the latest version.";
      break;
    default:
      return "Unknown.";
      break;
    }
}
