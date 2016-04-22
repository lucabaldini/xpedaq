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

#include "pUsbControllerBase.h"


pUsbControllerBase::pUsbControllerBase()
{
  *xpollog::kInfo << "Creating dummy USB controller..." << endline;
  setup();
}

pUsbControllerBase::~pUsbControllerBase()
{
  
}

unsigned long pUsbControllerBase::getLastErrorCode()
{
  return 0;
}

int pUsbControllerBase::readSetting(unsigned short address,
				    unsigned short *value)
{
  *xpollog::kDebug << "Reading address " << address << " (0x" << hex <<
    address << dec << ")... kidding." << flush;
  return 0;
}

int pUsbControllerBase::readData(unsigned char *data, unsigned long *length)
{
  return 0;
}

int pUsbControllerBase::writeSetting(unsigned short address,
				     unsigned short value)
{
  *xpollog::kDebug << "Writing " << value << " (0x" << hex << value << ")" <<
    dec << " to address " << address << " (0x" << hex << address << dec <<
    ")" << "... kidding." << flush;
   return 0;
}

int pUsbControllerBase::writePortDir(unsigned short address, unsigned char data)
{
  *xpollog::kDebug << "Writing port dir at address " << address << " (0x" <<
    hex << address << dec << ")... kidding." << flush;
  return 0;
}

int pUsbControllerBase::writePort(unsigned short address, unsigned char *data,
			      unsigned short length)
{
  *xpollog::kDebug << "Writing port at address " << address << " (0x"
		   << hex << address << dec << ")... kidding." << flush;
  return 0;
}

int pUsbControllerBase::setTimeout(unsigned long timeout)
{
  *xpollog::kDebug << "Setting timeout to " << timeout << " ms... kidding."
		   << endline;
  return 0;
}

int pUsbControllerBase::setup()
{
  return 1;
}

int pUsbControllerBase::readUsbSettings()
{
  *xpollog::kInfo << "Reading QuickUSB settings... kidding." << endline;
  return 0;
}

int pUsbControllerBase::writeUsbSettings()
{
  *xpollog::kInfo << "Writing QuickUSB settings... kidding." << endline;
  return 0;
}

int pUsbControllerBase::startSequencer()
{
  *xpollog::kInfo << "Starting QuickUsb sequencer... kidding." << endline;
  return 0;
}

int pUsbControllerBase::stopSequencer()
{
  *xpollog::kInfo << "Stopping QuickUsb sequencer... kidding." << endline;
  return 0;
}

int pUsbControllerBase::resetSequencer()
{
  *xpollog::kInfo << "Resetting QuickUsb sequencer... kidding." << endline;
  return 0;
}

int pUsbControllerBase::flushQUsbFIFO()
{
  *xpollog::kInfo << "Flushing QuickUsb FIFO... kidding." << endline;
  return 0;
}

bool pUsbControllerBase::IsOpened()
{
  return true;
}

int pUsbControllerBase::ReadPort(unsigned short address, unsigned char *data,
	       unsigned short *length)
{
  return 0;
}

int pUsbControllerBase::WritePort(unsigned short address, unsigned char *data,
		unsigned short length)
{
  return 0;
}
