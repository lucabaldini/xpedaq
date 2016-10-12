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

#ifndef PEVENTREADER_H
#define PEVENTREADER_H

#include <vector>

#include <QMutex>
#include <QUdpSocket>
#include <QHostAddress>

#include "pDataBlock.h"
#include "pHistogram.h"
#include "pMap.h"
#include "pEvent.h"
#include "pMonitorPreferences.h"

class pEventReader: public QObject
{

  Q_OBJECT
  
  public:
  
    pEventReader(const pMonitorPreferences& preferences,
                 pHistogram* pulseHeightHist, pHistogram* windowSizeHist,
                 pHistogram* modulationHist, pMap* hitMap);
  
  public slots:
  
    void startReading();
    void readPendingDatagrams();
    void updateRequested();
    void setStopped();
    void updatePreferences(const pMonitorPreferences& preferences);
  
  signals:
  
    void stopped();
    void eventRead();
    void lastEventUpdated(const pEvent& evt);
    void pulseHeightHistUpdated();
    void windowSizeHistUpdated();
    void modulationHistUpdated();
    void hitMapUpdated();
  
  private:
    
    void readPendingDatagram();
    bool evtAccepted(const pEvent& evt);
  
    //Data structures
    pHistogram *m_pulseHeightHist;
    pHistogram *m_windowSizeHist;
    pHistogram *m_modulationHist;
    pMap *m_hitMap;
    
    //Current event info:
    pEvent m_lastEvent;
      
    pMonitorPreferences m_preferences;
    bool m_stopped;
    bool m_isContentChanged;
    QMutex m_mutex;
    QUdpSocket* m_udpSocket;
    
};

#endif //PEVENTREADER_H

