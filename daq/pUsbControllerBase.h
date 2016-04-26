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

#ifndef PUSBCONTROLLERBASE_H
#define PUSBCONTROLLERBASE_H

#include <iostream>

#include "xpollog.h"


class pUsbControllerBase
{

 public:
  pUsbControllerBase();
  pUsbControllerBase(const char *devName);
  ~pUsbControllerBase();
  int setup();
  int readUsbSettings();
  int writeUsbSettings();
  int startSequencer();
  int stopSequencer();
  int resetSequencer();
  int flushQUsbFIFO();
  unsigned long getLastErrorCode();
  int readSetting(unsigned short address, unsigned short *value);
  int readData(unsigned char *data, unsigned long *length);
  int writeSetting(unsigned short address, unsigned short value);
  int writePortDir(unsigned short address, unsigned char data);
  int writePort(unsigned short address, unsigned char *data,
		unsigned short length);
  int setTimeout(unsigned long timeout);
  // QuickUsb "emulation".
  bool IsOpened();
  int ReadPort(unsigned short address, unsigned char *data,
	       unsigned short *length);
  int WritePort(unsigned short address, unsigned char *data,
		unsigned short length);
 protected:

 private:

};

#endif
