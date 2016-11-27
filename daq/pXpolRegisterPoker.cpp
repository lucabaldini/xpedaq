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


#include "pXpolRegisterPoker.h"


/*!
 */
pXpolRegisterPoker::pXpolRegisterPoker(pXpolFpga *xpolFpga) :
  m_xpolFpga(xpolFpga),
  m_pixelAddressX(0),
  m_pixelAddressY(0),
  m_configuration(0),
  m_randomShuffle(false),
  m_readoutRepeat(100),
  m_readoutInterval(100),
  m_numPokes(0),
  m_numReadouts(0),
  m_numReadoutErrors(0)
{
  m_rndEngine = std::default_random_engine();
  m_rndDistX = std::uniform_int_distribution<int>(0, 299);
  m_rndDistY = std::uniform_int_distribution<int>(0, 351);
  m_rndDistConfig = std::uniform_int_distribution<int>(0, 65535);
}


/*! Setup the register poker passing all the arguments explicitely.
 */
void pXpolRegisterPoker::setup(unsigned short x, unsigned short y,
			       unsigned short config, bool shuffle, int repeat,
			       int interval)
{
  m_pixelAddressX = x;
  m_pixelAddressY = y;
  m_configuration = config;
  m_randomShuffle = shuffle;
  m_readoutRepeat = repeat;
  m_readoutInterval = interval;
}


/*! Setup the register poker via a xperegUserPreferences pointer.
 */
void pXpolRegisterPoker::setup(xperegUserPreferences *preferences)
{
  setup(preferences->m_pixelAddressX, preferences->m_pixelAddressY,
	preferences->m_configuration, preferences->m_randomShuffle,
	preferences->m_readoutRepeat, preferences->m_readoutInterval);
}


/*! Shuffle the values being written into the registers randomly.
 */
void pXpolRegisterPoker::shuffle()
{
  m_pixelAddressX = m_rndDistX(m_rndEngine);
  m_pixelAddressY = m_rndDistY(m_rndEngine);
  m_configuration = m_rndDistConfig(m_rndEngine);
}


/*! Reset the test statistics.
 */
void pXpolRegisterPoker::reset()
{
  m_numPokes = 0;
  m_numReadouts = 0;
  m_numReadoutErrors = 0;
}


/*! Write the XPOL registers once .
 */
void pXpolRegisterPoker::write()
{
  *xpollog::kInfo << "Writing (" << m_pixelAddressX << ", " << m_pixelAddressY
		  << "), 0x" << hex << m_configuration << dec
		  << " into the XPOL registers..." << endline;
  m_xpolFpga->writeXpolAddressRegister(m_pixelAddressX, m_pixelAddressY);
  m_xpolFpga->writeXpolConfigurationRegister(m_configuration);
  emit registersWritten(m_pixelAddressX, m_pixelAddressY, m_configuration);
}


/*! Read back the XPOL registers and compare with the values that we wrote.

The function returns 0 upon success and an error code upon failure. The actual
readout values are stored into the references passed as arguments.
 */
int pXpolRegisterPoker::read(unsigned short &x, unsigned short &y,
			     unsigned short &config)
{
  int errorCode = 0;
  m_xpolFpga->readXpolAddressConfigurationRegisters(x, y, config);
  if (x != m_pixelAddressX) {
    errorCode += AddressXMismatch;
  }
  if (y != m_pixelAddressY) {
    errorCode += AddressYMismatch;
  }
  if (config != m_configuration) {
    errorCode += ConfigurationMismatch;
  }
  if (errorCode) {
    emit readoutError(errorCode);
    m_numReadoutErrors += 1;
    *xpollog::kError << "Readout error code 0x" << hex << errorCode
		     << dec << ", actual readout values (" << x << ", "
		     << y << "), " << hex << config << dec << "." << endline;
  }
  return errorCode; 
}


/*!
 */
void pXpolRegisterPoker::poke()
{
  unsigned short x, y, config;
  if (m_numReadouts % m_readoutRepeat == 0) {
    if (m_randomShuffle) {
      shuffle();
    }
    write();
    m_numPokes += 1;
  }
  read(x, y, config);
  m_numReadouts += 1;
}


/*!
 */
void pXpolRegisterPoker::start()
{
  m_timer = new QTimer();
  m_timer->setInterval(100);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(poke()));
  m_timer->start();
}


/*!
 */
void pXpolRegisterPoker::stop()
{
  m_timer->stop();
  delete m_timer;
}
