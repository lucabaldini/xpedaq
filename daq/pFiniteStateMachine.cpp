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

#include "pFiniteStateMachine.h"


pFiniteStateMachine::pFiniteStateMachine()
{
  m_statusCode = UNDEFINED;
}

int pFiniteStateMachine::statusCode()
{
  return m_statusCode;
}

int pFiniteStateMachine::isReset()
{
  return m_statusCode == RESET;
}

int pFiniteStateMachine::isStopped()
{
  return m_statusCode == STOPPED;
}

int pFiniteStateMachine::isRunning()
{
  return m_statusCode == RUNNING;
}

int pFiniteStateMachine::isPaused()
{
  return m_statusCode == PAUSED;
}

QString pFiniteStateMachine::status()
{
  switch(m_statusCode) {
  case RESET:
    return "RESET";
    break;
  case STOPPED:
    return "STOPPED";
    break;
  case RUNNING:
    return "RUNNING";
    break;
  case PAUSED:
    return "PAUSED";
    break;
  default:
    return "UNDEFINED";
  }
}

void pFiniteStateMachine::setRunning()
{
  switch(m_statusCode) {
  case RESET:
    m_statusCode = RUNNING;
    emit statusChanged(status());
    fsmSetup();
    fsmStartRun();
    break;
  case STOPPED:
    m_statusCode = RUNNING;
    emit statusChanged(status());
    fsmStartRun();
    break;
  case PAUSED:
    m_statusCode = RUNNING;
    emit statusChanged(status());
    fsmResume();
    break;
  default:
    ;
  }
}

void pFiniteStateMachine::setStopped()
{
  switch(m_statusCode) {
  case RESET:
    m_statusCode = STOPPED;
    emit statusChanged(status());
    fsmSetup();
    break;
  case RUNNING:
    m_statusCode = STOPPED;
    emit statusChanged(status());
    fsmStopRun();
    break;
  case PAUSED:
    m_statusCode = STOPPED;
    emit statusChanged(status());
    fsmStop();
    break;
  default:
    ;
  }
}

void pFiniteStateMachine::setPaused()
{
  switch(m_statusCode) {
  case RUNNING:
    m_statusCode = PAUSED;
    emit statusChanged(status());
    fsmPause();
    break;
  default:
    ;
  }
}

void pFiniteStateMachine::setReset()
{
  switch(m_statusCode) {
  case STOPPED:
    m_statusCode = RESET;
    emit statusChanged(status());
    fsmTeardown();
    break;
  case UNDEFINED:
    m_statusCode = RESET;
    emit statusChanged(status());
  default:
    ;
  }
}
