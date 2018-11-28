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

#include "pEventReader.h"

pEventReader::pEventReader(const pMonitorPreferences& preferences,
			                     pHistogram* windowSizeHist,
			                     pHistogram* clusterSizeHist,
                                 pHistogram* pulseHeightHist, 
                                 pModulationHistogram* modulationHist,
                                 pMap* hitMap) :
  m_windowSizeHist(windowSizeHist),
  m_clusterSizeHist(clusterSizeHist),
  m_pulseHeightHist(pulseHeightHist),
  m_modulationHist(modulationHist),
  m_hitMap(hitMap),
  m_preferences(preferences),
  m_numEventsRead(-1),
  m_numEventsAccepted(-1),
  m_startSeconds(-1)
{
  m_udpSocket = new QUdpSocket (this);
}


long int pEventReader::currentSeconds() const
{
  return static_cast<long int> (time(NULL));
}


void pEventReader::readPendingDatagram()
{
  qint64 size = m_udpSocket->pendingDatagramSize();
  //we need a char* because of QUdpSocket->readDatagram() signature  
  char* data = new (std::nothrow) char[size];
  if (data == nullptr)
  {
    std::cout << "allocation failed" << std::endl;
    return;
  }
  m_udpSocket->readDatagram(data, size);
  
  /* When instantiating a pDataBlock we pass to its constructor a pointer
     to the buffer - no actual copy of the data involved.
  */
  pDataBlock p (reinterpret_cast<unsigned char*> (data), size);
  if (p.errorSummary() > 0){
    std::cout << "Invalid datablock: " << p;
    return;
  }
  for (unsigned int evt = 0; evt < p.numEvents(); ++evt)
  {
    int nPixel = p.numPixels(evt);
    event::Adc_vec_t curHitMap;
    curHitMap.resize(nPixel);
    unsigned int x = 1000; //initialize to non-physical value
    unsigned int y = 1000; //initialize to non-physical value
    adc_count_t height = 0;
    for (int index = 0; index < nPixel; ++index)
    {
      p.readPixel(evt, index, x, y, height);
      //zero suppression
      if (height > m_preferences.m_zeroSuppressionThreshold)
        curHitMap.at(index) = height;
      else
        curHitMap.at(index) = 0;
    }
    pEvent event = pEvent(p.xmin(evt), p.xmax(evt), p.ymin(evt), p.ymax(evt),
			   curHitMap, p.microseconds(evt),
			   m_preferences.m_zeroSuppressionThreshold);
    if (!m_preferences.m_skipReconstruction) {
      event.reconstruct(m_preferences.m_zeroSuppressionThreshold);
    }
    emit eventRead();
    m_numEventsRead += 1;
    if (eventAccepted(event)) {
      m_numEventsAccepted += 1;
      m_isLastEventChanged = true;    
      m_lastEvent = event;
      for (auto const& it : event){
        if ((it.clusterId == 0) || (m_preferences.m_skipReconstruction)){
          OffsetCoordinate coord = event.coordToPixel(it.x, it.y);
          m_hitMap->fill(coord.col(), coord.row(),
                         static_cast<double> (it.counts));
        }
      }
      m_windowSizeHist->fill(nPixel);
      m_clusterSizeHist->fill(event.clusterSize());
      m_pulseHeightHist->fill(event.pulseHeight());
      m_modulationHist->fill(event.phiDeg());
    }
  }
}


bool pEventReader::eventAccepted(const pEvent& event)
{
  if (m_preferences.m_skipReconstruction) {
    return (event.evtSize() > m_preferences.m_minWindowSize &&
            event.evtSize() < m_preferences.m_maxWindowSize &&
            event.pulseHeight() > m_preferences.m_minPulseHeight &&
            event.pulseHeight() < m_preferences.m_maxPulseHeight);
  } else {
    return (event.clusterSize() > m_preferences.m_minClusterSize &&
            event.clusterSize() < m_preferences.m_maxClusterSize &&
            event.pulseHeight() > m_preferences.m_minPulseHeight &&
            event.pulseHeight() < m_preferences.m_maxPulseHeight &&
            event.moma1().elongation() > m_preferences.m_minElongation &&
            event.moma1().elongation() < m_preferences.m_maxElongation &&
            event.evtSize() > m_preferences.m_minWindowSize &&
            event.evtSize() < m_preferences.m_maxWindowSize &&
	        fabs(event.skewness()) > m_preferences.m_minSkewness &&
	        fabs(event.skewness()) < m_preferences.m_maxSkewness);
  }
} 


void pEventReader::readPendingDatagrams()
{  
  QMutexLocker locker(&m_mutex);
  while ((!m_stopped) && (m_udpSocket->hasPendingDatagrams()))
  {
    readPendingDatagram();
  }
}


void pEventReader::updateRequested()
{
  QMutexLocker locker(&m_mutex);
  if (m_isLastEventChanged) 
    emit lastEventUpdated(m_lastEvent);
  emit histogramsUpdated();
  m_isLastEventChanged = false;
}


void pEventReader::startReading()
{
  // Reset the stats.
  m_numEventsRead = 0;
  m_numEventsAccepted = 0;
  m_startSeconds = currentSeconds();
  QMutexLocker locker(&m_mutex);
  m_stopped = false;
  m_isLastEventChanged = false;
  m_udpSocket->bind(m_preferences.m_socketPort);
  connect(m_udpSocket, SIGNAL(readyRead()), 
          this, SLOT(readPendingDatagrams()));
}


void pEventReader::setStopped()
{
  QMutexLocker locker(&m_mutex);
  m_udpSocket->disconnectFromHost();
  m_stopped = true;
  emit stopped();
}


void pEventReader::updatePreferences(const pMonitorPreferences& preferences)
{
  QMutexLocker locker(&m_mutex);
  m_preferences = preferences;
}
