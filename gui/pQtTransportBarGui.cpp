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

#include "pQtTransportBarGui.h"

pQtTransportBarGui::pQtTransportBarGui(QWidget *parent, bool minimal) :
  pQtGroupBoxWidget(parent),
  m_resetButton(NULL),
  m_stopButton(NULL),
  m_pauseButton(NULL),
  m_startButton(NULL)
{
  setupButtons(minimal);
  setTitle("Transport bar");
  freezeSize();
}

void pQtTransportBarGui::setupButtons(bool minimal)
{
  if (!minimal) {
    m_resetButton = new pQtCustomPushButton(m_mainGroupBox, "reset");
    m_pauseButton = new pQtCustomPushButton(m_mainGroupBox, "pause");
  }
  m_stopButton  = new pQtCustomPushButton(m_mainGroupBox, "stop");
  m_startButton = new pQtCustomPushButton(m_mainGroupBox, "start");
  if (m_resetButton != NULL) {
    addWidget(m_resetButton, 0, 0);
  }
  addWidget(m_stopButton , 0, 1);
  if (m_pauseButton != NULL) {
    addWidget(m_pauseButton, 0, 2);
  }
  addWidget(m_startButton, 0, 3);
}

void pQtTransportBarGui::enable()
{
  if (m_resetButton != NULL) {
    m_resetButton->setEnabled(1);
  }
  m_stopButton->setEnabled(1);
  if (m_pauseButton != NULL) {
    m_pauseButton->setEnabled(1);
  }
  m_startButton->setEnabled(1);
}

void pQtTransportBarGui::disable()
{
  if (m_resetButton != NULL) {
    m_resetButton->setEnabled(0);
  }
  m_stopButton->setEnabled(0);
  if (m_pauseButton != NULL) {
    m_pauseButton->setEnabled(0);
  }
  m_startButton->setEnabled(0);
}
