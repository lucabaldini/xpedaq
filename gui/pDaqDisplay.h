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

#ifndef PDAQDISPLAY_H
#define PDAQDISPLAY_H

#include <iostream>

#include <QString>

#include "pQtGroupBoxWidget.h"
#include "pQtCustomTextLabel.h"
#include "pQtDisplayGui.h"

/*! \brief Widget for displaying data acquisition related information.

\todo Could be implemented better with a stl container of pQtCustomTextLabel objects and
a setField() method, rather than a lot of members and methods explicitely defined.*/

class pDaqDisplay : public pQtDisplayGui
{

  Q_OBJECT

 public:
  pDaqDisplay(QWidget *parent);
  ~pDaqDisplay() {;}

 public slots:
  inline int stationId() const
    {return m_stationIdLabel->text().toInt();}
  void updateStationId(int stationId)
    {m_stationIdLabel->setText(stationId);}
  inline int runId() const
    {return m_runIdLabel->text().toInt();}
  void updateRunId(int runId)
    {m_runIdLabel->setText(runId);}
  void updateStatus(QString status)
    {m_statusLabel->setText(status);}
  void updateNumDataBlocks(int numDataBlocks)
    {m_numDataBlocksLabel->setText(numDataBlocks);}
  void updateNumEvents(int numEvents)
    {m_numEventsLabel->setText(numEvents);}
  void updateElapsedSeconds(int elapsedSeconds)
    {m_elapsedTimeLabel->setText(elapsedSeconds);}
  void updateAverageDaqRate(double rate)
    {m_averageDaqEventRateLabel->setText(rate);}
  void updateInstantDaqRate(double rate)
    {m_instantDaqEventRateLabel->setText(rate);}
  void updateInstantFpgaRate(double rate)
    {m_instantFpgaEventRateLabel->setText(rate);}

 private:
  pQtCustomTextLabel *m_stationIdLabel;
  pQtCustomTextLabel *m_runIdLabel;
  pQtCustomTextLabel *m_statusLabel;
  pQtCustomTextLabel *m_numDataBlocksLabel;
  pQtCustomTextLabel *m_numEventsLabel;
  pQtCustomTextLabel *m_elapsedTimeLabel;
  pQtCustomTextLabel *m_averageDaqEventRateLabel;
  pQtCustomTextLabel *m_instantDaqEventRateLabel;
  pQtCustomTextLabel *m_instantFpgaEventRateLabel;
};

#endif
