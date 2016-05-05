/***********************************************************************
Copyright (C) 2007--2016 the X-ray Polarimetry Explorer (XPE) team.

For the license terms see the file LICENSE, distributed along with this
software.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
***********************************************************************/

#ifndef PFINITESTATEMACHINE_H
#define PFINITESTATEMACHINE_H

#include <QObject>
#include <QString>


/*! \brief Base finite state machine underlying the DAQ system.

  The finite state machine has five states: UNDEFINED, RESET, STOPPED, RUNNING
  and PAUSED. The implementation defines the standard operations for the
  transitions between such states.
*/

class pFiniteStateMachine : public QObject
{

  Q_OBJECT

 public:
  
  pFiniteStateMachine();
  virtual ~pFiniteStateMachine() {;}
  int statusCode();
  int isReset();
  int isStopped();
  int isRunning();
  int isPaused();
  QString status();

  
 signals:

  void statusChanged(QString status);
  

 public slots:
   
  void setRunning();
  void setStopped();
  void setPaused();
  void setReset();

  
 protected:

  enum StatusCode
  {
    UNDEFINED,
    RESET,
    STOPPED,
    RUNNING,
    PAUSED
  };
  int m_statusCode;
  virtual void fsmSetup() = 0;
  virtual void fsmTeardown() = 0;
  virtual void fsmStartRun() = 0;
  virtual void fsmStopRun() = 0;
  virtual void fsmPause() = 0;
  virtual void fsmResume() = 0;
  virtual void fsmStop() = 0;
};

#endif //PFINITESTATEMACHINE_H
