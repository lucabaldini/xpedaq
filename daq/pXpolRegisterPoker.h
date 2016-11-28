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

#ifndef PXPOLREGISTERPOKER_H
#define PXPOLREGISTERPOKER_H

#include <random>

#include <QTimer>

#include "pXpolFpga.h"
#include "xperegUserPreferences.h"
#include "xpollog.h"


class pXpolRegisterPoker: public QObject
{

  Q_OBJECT
  
  public:
  
    pXpolRegisterPoker(pXpolFpga *xpolFpga);
    void setup(unsigned short x, unsigned short y, unsigned short config,
	       bool shuffle, int repeat, int interval);
    void setup(xperegUserPreferences *preferences);
    void shuffle();
    void reset();
    void write();
    int read(unsigned short &x, unsigned short &y, unsigned short &config);
    // Access test statistics.
    int numPokes() const {return m_numPokes;}
    int numReadouts() const {return m_numReadouts;}
    int numReadoutErrors() const {return m_numReadoutErrors;}

    
  public slots:

    void poke();
    void start();
    void stop();
    
  signals:

    void registersWritten(unsigned short x, unsigned short y,
			  unsigned short config);
    void shuffled(unsigned short x, unsigned short y, unsigned short config);
    void readoutError(int errorCode);
  
  private:

    enum ReadoutError {
      AddressXMismatch = 0x1,
      AddressYMismatch = 0x2,
      ConfigurationMismatch = 0x4
    };
    
    QTimer *m_timer;
    pXpolFpga *m_xpolFpga;
    // Test configuration.
    unsigned short m_pixelAddressX;
    unsigned short m_pixelAddressY;
    unsigned short m_configuration;
    bool m_randomShuffle;
    int m_readoutRepeat;
    int m_readoutInterval;
    // Bookkeping for readout statistics and errors.
    int m_numPokes;
    int m_numReadouts;
    int m_numReadoutErrors;
    // Random number generation.
    std::default_random_engine m_rndEngine;
    std::uniform_int_distribution<int> m_rndDistX;
    std::uniform_int_distribution<int> m_rndDistY;
    std::uniform_int_distribution<int> m_rndDistConfig;
};

#endif //PXPOLREGISTERPOKER_H

