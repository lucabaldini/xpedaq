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

#include "pTransportBar.h"


pTransportBar::pTransportBar(QWidget *parent)
  : pQtTransportBarGui(parent)
{
  setupConnections();
  setupToolTips();
  setReset();
}

void pTransportBar::pressStartButton()
{
  startButtonPressed();
}

void pTransportBar::pressStopButton()
{
  stopButtonPressed();
}

void pTransportBar::setupConnections()
{
  connect(m_startButton, SIGNAL(clicked()), this, SLOT(startButtonPressed()));
  connect(m_stopButton,  SIGNAL(clicked()), this, SLOT(stopButtonPressed()));
  connect(m_pauseButton, SIGNAL(clicked()), this, SLOT(pauseButtonPressed()));
  connect(m_resetButton, SIGNAL(clicked()), this, SLOT(resetButtonPressed()));
}

void pTransportBar::setupToolTips()
{
  m_startButton->setToolTip("Start the data acquisition");
  m_stopButton->setToolTip("Stop the data acquisition");
  m_pauseButton->setToolTip("Pause the data acquisition");
  m_resetButton->setToolTip("Reset the run controller");
}

void pTransportBar::startButtonPressed()
{
  emit start();
  setRunning();
}

void pTransportBar::stopButtonPressed()
{
  emit stop();
  setStopped();
}

void pTransportBar::pauseButtonPressed()
{
  emit pause();
  setPaused();
}

void pTransportBar::resetButtonPressed()
{
  emit reset();
  setReset();
}

void pTransportBar::setRunning()
{
  m_resetButton->setEnabled(0);
  m_stopButton->setEnabled(1);
  m_pauseButton->setEnabled(1);
  m_startButton->setEnabled(0);
}

void pTransportBar::setPaused()
{
  m_resetButton->setEnabled(0);
  m_stopButton->setEnabled(1);
  m_pauseButton->setEnabled(0);
  m_startButton->setEnabled(1);
}

void pTransportBar::setStopped()
{
  m_resetButton->setEnabled(1);
  m_stopButton->setEnabled(0);
  m_pauseButton->setEnabled(0);
  m_startButton->setEnabled(1);
}

void pTransportBar::setReset()
{
  m_resetButton->setEnabled(0);
  m_stopButton->setEnabled(0);
  m_pauseButton->setEnabled(0);
  m_startButton->setEnabled(1);
}
