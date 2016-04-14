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

#include "pQtTransportBarGui.h"

pQtTransportBarGui::pQtTransportBarGui(QWidget *parent)
  : pQtGroupBoxWidget(parent)
{
  setupButtons();
  setTitle("Transport bar");
  freezeSize();
}

void pQtTransportBarGui::setupButtons()
{
  m_resetButton = new pQtCustomPushButton(m_mainGroupBox, "reset");
  m_stopButton  = new pQtCustomPushButton(m_mainGroupBox, "stop");
  m_pauseButton = new pQtCustomPushButton(m_mainGroupBox, "pause");
  m_startButton = new pQtCustomPushButton(m_mainGroupBox, "start");
  addWidget(m_resetButton, 0, 0);
  addWidget(m_stopButton , 0, 1);
  addWidget(m_pauseButton, 0, 2);
  addWidget(m_startButton, 0, 3);
}

void pQtTransportBarGui::enable()
{
  m_resetButton->setEnabled(1);
  m_stopButton->setEnabled(1);
  m_pauseButton->setEnabled(1);
  m_startButton->setEnabled(1);
}

void pQtTransportBarGui::disable()
{
  m_resetButton->setEnabled(0);
  m_stopButton->setEnabled(0);
  m_pauseButton->setEnabled(0);
  m_startButton->setEnabled(0);
}
