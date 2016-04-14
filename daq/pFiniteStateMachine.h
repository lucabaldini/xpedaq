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

#ifndef PFINITESTATEMACHINE_H
#define PFINITESTATEMACHINE_H

#include <iostream>
#include <QObject>
#include <QString>
#include "xpollog.h"


const int kUndefinedStatusCode = 0x0;
const int kResetStatusCode = 0x1;
const int kStoppedStatusCode = 0x2;
const int kRunningStatusCode = 0x4;
const int kPausedStatusCode = 0x8;


/*! \brief Base finite state machine underlying the DAQ system.

  The FSM essentially is always in one of five states: UNDEFINED, RESET,
  STOPPED, RUNNING and PAUSED.
  The implementation defined the standard operations for state stransitions.

  \todo Include the status codes definition into an enum.
*/

class pFiniteStateMachine : public QObject
{

  Q_OBJECT

 public:
  
  /*! \brief Basic constructor.*/
  pFiniteStateMachine();
  /*! \brief Destructor.*/
  virtual ~pFiniteStateMachine();
  /*! \brief Return the status code.*/
  int getStatus();
  /*! \brief Return true if the FSM is in the RESET state.*/
  int isReset();
  /*! \brief Return true if the FSM is in the STOPPED state.*/
  int isStopped();
  /*! \brief Return true if the FSM is in the RUNNING state.*/
  int isRunning();
  /*! \brief Return true if the FSM is in the PAUSED state.*/
  int isPaused();
  /*! \brief Return the status string corresponding to the status code.*/
  QString getStatusString();

 signals:

  /*! \brief Signal emitted whenever the status changes.
    
    The status string which is passed along with the signal can be used
    for displaying on the main GUI the current status of the FSM.
  */
  void statusChanged(QString status);

 public slots:

  /*! \brief Start the FSM.*/
  void start();
  /*! \brief Stop the FSM.*/
  void stop();
  /*! \brief Pause the FSM.*/
  void pause();
  /*! \brief Reset the FSM.*/
  void reset();

 protected:

  /*! \brief The status code.*/
  int m_statusCode;
  /*! \brief Function called when passing from RESET to STOPPED.*/
  virtual void fsm_setup();
  /*! \brief Function called when passing from STOPPED to RESET.*/
  virtual void fsm_teardown();
  /*! \brief Function called when passing from STOPPED to RUNNING.*/
  virtual void fsm_startRun();
  /*! \brief Function called when passing from RUNNING to STOPPED.*/
  virtual void fsm_stopRun();
  /*! \brief Function called when passing from RUNNING to PAUSED.*/
  virtual void fsm_pause();
  /*! \brief Function called when passing from PAUSED to RUNNING.*/
  virtual void fsm_resume();
  /*! \brief Function called when passing from PAUSED to STOPPED.*/
  virtual void fsm_stop();
};

#endif
