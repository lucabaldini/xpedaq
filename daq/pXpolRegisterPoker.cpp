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


pXpolRegisterPoker::pXpolRegisterPoker(pXpolFpga *xpolFpga) :
  m_xpolFpga(xpolFpga)
{
}


void pXpolRegisterPoker::reset()
{

}

void pXpolRegisterPoker::poke()
{
  m_xpolFpga->writeXpolConfigurationRegister(0x23);
  m_xpolFpga->writeXpolAddressRegister(50, 77);
  *xpollog::kInfo << "Reading back XPOL registers..." << endline; 
  unsigned short x, y, value;
  m_xpolFpga->readXpolAddressConfigurationRegisters(x, y, value);
  *xpollog::kInfo << "Address x: " << x << endline;
  *xpollog::kInfo << "Address y: " << y << endline;
  *xpollog::kInfo << "Configuration register: 0x" << hex << value << dec
		  << endline;
}


void pXpolRegisterPoker::start()
{
  m_timer = new QTimer();
  m_timer->setInterval(100);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(poke()));
  m_timer->start();
}



void pXpolRegisterPoker::stop()
{
  m_timer->stop();
  delete m_timer;
}
