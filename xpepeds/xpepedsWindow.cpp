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


#include "xpepedsWindow.h"
#include <iostream>


xpepedsWindow::xpepedsWindow(pedRunController &runController) :
  pAcquisitionWindow(runController), m_pedRunController(&runController)
{
  setupConnections();
  QString title = "xpepeds version " + QString(__XPEDAQ_VERSION__);
  setWindowTitle(title);
  connect (m_transportBar, SIGNAL(stop()), this, SLOT(showDisplayWindow()));
  m_readoutModeTab -> disableAll();
}

/*!
 */
void xpepedsWindow::showDisplayWindow()
{
  m_pedRunController -> randomFilling(10);
  m_displayWindow = new pDisplayWindow();
  m_displayWindow -> show();
  m_displayWindow -> showPedestals(m_pedRunController -> pedMap());
}

/*!
 */
void xpepedsWindow::setupConnections()
{
  pAcquisitionWindow::setupConnections();
  connect(m_transportBar, SIGNAL(start()), this, SLOT(startRun()));
}
