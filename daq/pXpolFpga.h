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

#ifndef PXPOLFPGA_H_
#define PXPOLFPGA_H_

#include "xpollog.h"
#include "xpoldetector.h"
#include "pUsbController.h"
#include "pDetectorConfiguration.h"
#include "pTriggerMask.h"

class pXpolFpga : public QObject
{

  Q_OBJECT

 public:
  pXpolFpga(pUsbController *usbController);
  ~pXpolFpga();

  void setup(pDetectorConfiguration *configuration); 
  void configFullFrame();
  void configWindowedMode(pDetectorConfiguration *configuration);
  void configXPM();
  void configXPMWindowed(pDetectorConfiguration *configuration);
  void writeAddress(unsigned short X, unsigned short Y);
  
  pUsbController *usbController() const {return m_usbController;}

  void setDacThreshold(pDetectorConfiguration *configuration);
  void setDacConfig();
  void configDAC(pDetectorConfiguration *configuration);

  // Trigger mask-related functions.
  void enableAllPixels();
  void maskPixel(unsigned short x, unsigned short y);
  void applyTriggerMask(pTriggerMask *trgMask);

  int resetSequencer(); // Currently Not used : Low level FPGA sequencer
 
 protected:

 private:

   void serialWrite(unsigned short REG_ADD,unsigned short regdata);
   void mainSerialWrite(unsigned short REG_ADD,unsigned short regdata);
   void setupToDisablePixels();
   unsigned short mainSiRead(unsigned short reg_address);
   pUsbController *m_usbController;

 public slots:
   unsigned short readVrefDac();


 signals:
   void vrefRead(unsigned short dac, double vref);

};

#endif /*PXPOLFPGA_H_*/
