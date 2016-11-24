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


#include "xperegRunController.h"


xperegRunController::xperegRunController()
{
  m_stationIdFilePath = xpedaqos::rjoin("config", "stationId.cfg");
  if (!xpedaqos::fileExists(m_stationIdFilePath)) {
    *xpollog::kError << "Could not find " << m_stationIdFilePath << endline;
    exit(1);
  }
  m_runIdFilePath = xpedaqos::rjoin("config", "runId.cfg");
  setupRun();
  m_timer = new QTimer();
  m_timer->setInterval(1000);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(updateRunInfo()));
  m_usbController = new pUsbController();
  m_xpolFpga = new pXpolFpga(m_usbController);
  // This is not used and we should get rid of it?
  m_dataCollector = new pDataCollector(m_xpolFpga, false);
}                                   


void xperegRunController::setupRun()
{
  std::cout << "test." << std::endl;
}
