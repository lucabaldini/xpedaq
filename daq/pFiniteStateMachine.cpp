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

#include "pFiniteStateMachine.h"

/*! The FSM is put into the UNDEFINED status ipon creation.*/

pFiniteStateMachine::pFiniteStateMachine()
{
  m_statusCode = kUndefinedStatusCode;
}

/*! Do nothing destructor.*/

pFiniteStateMachine::~pFiniteStateMachine()
{
  
}

int pFiniteStateMachine::getStatus()
{
  return m_statusCode;
}

int pFiniteStateMachine::isReset()
{
  return m_statusCode == kResetStatusCode;
}

int pFiniteStateMachine::isStopped()
{
  return m_statusCode == kStoppedStatusCode;
}

int pFiniteStateMachine::isRunning()
{
  return m_statusCode == kRunningStatusCode;
}

int pFiniteStateMachine::isPaused()
{
  return m_statusCode == kPausedStatusCode;
}

QString pFiniteStateMachine::getStatusString()
{
  switch(m_statusCode){
  case kResetStatusCode:
    return "RESET";
    break;
  case kStoppedStatusCode:
    return "STOPPED";
    break;
  case kRunningStatusCode:
    return "RUNNING";
    break;
  case kPausedStatusCode:
    return "PAUSED";
    break;
  default:
    return "UNDEFINED";
  }
}

void pFiniteStateMachine::start()
{
  switch(m_statusCode){
  case kResetStatusCode:
    m_statusCode = kRunningStatusCode;
    emit statusChanged(getStatusString());
    fsm_setup();
    fsm_startRun();
    break;
  case kStoppedStatusCode:
    m_statusCode = kRunningStatusCode;
    emit statusChanged(getStatusString());
    fsm_startRun();
    break;
  case kPausedStatusCode:
    m_statusCode = kRunningStatusCode;
    emit statusChanged(getStatusString());
    fsm_resume();
    break;
  default:
    ;
  }
}

void pFiniteStateMachine::stop()
{
  switch(m_statusCode){
  case kResetStatusCode:
    m_statusCode = kStoppedStatusCode;
    emit statusChanged(getStatusString());
    fsm_setup();
    break;
  case kRunningStatusCode:
    m_statusCode = kStoppedStatusCode;
    emit statusChanged(getStatusString());
    fsm_stopRun();
    break;
  case kPausedStatusCode:
    m_statusCode = kStoppedStatusCode;
    emit statusChanged(getStatusString());
    fsm_stop();
    break;
  default:
    ;
  }
}

void pFiniteStateMachine::pause()
{
  switch(m_statusCode){
  case kRunningStatusCode:
    m_statusCode = kPausedStatusCode;
    emit statusChanged(getStatusString());
    fsm_pause();
    break;
  default:
    ;
  }
}

void pFiniteStateMachine::reset()
{
  switch(m_statusCode){
  case kStoppedStatusCode:
    m_statusCode = kResetStatusCode;
    emit statusChanged(getStatusString());
    fsm_teardown();
    break;
  case kUndefinedStatusCode:
    m_statusCode = kResetStatusCode;
    emit statusChanged(getStatusString());
  default:
    ;
  }
}

void pFiniteStateMachine::fsm_setup()
{
  *xpollog::kDebug << "fsm_setup() not implemented." << endline;
}

void pFiniteStateMachine::fsm_teardown()
{
  *xpollog::kDebug << "fsm_teardown() not implemented." << endline;
}

void pFiniteStateMachine::fsm_startRun()
{
  *xpollog::kDebug << "fsm_startRun() not implemented." << endline;
}

void pFiniteStateMachine::fsm_stopRun()
{
  *xpollog::kDebug << "fsm_stopRun() not implemented." << endline;
}

void pFiniteStateMachine::fsm_pause()
{
  *xpollog::kDebug << "fsm_pause() not implemented." << endline;
}

void pFiniteStateMachine::fsm_resume()
{
  *xpollog::kDebug << "fsm_resume() not implemented." << endline;
}

void pFiniteStateMachine::fsm_stop()
{
  *xpollog::kDebug << "fsm_stop() not implemented." << endline;
}
