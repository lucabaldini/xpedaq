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
                           pHistogram* modulationHist,
                           pMap* hitMap) :
  m_windowSizeHist(windowSizeHist),
  m_clusterSizeHist(clusterSizeHist),
  m_pulseHeightHist(pulseHeightHist),
  m_modulationHist(modulationHist),
  m_hitMap(hitMap),
  m_preferences(preferences)
{
  m_udpSocket = new QUdpSocket (this);
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
     Since the destroyer of the pDataBlock does not delete the buffer we
     need to do this manually at the end of the functon (se issue #120).
  */
  pDataBlock p (reinterpret_cast<unsigned char*> (data), size);
  for (unsigned int evt = 0; evt < p.numEvents(); ++evt)
  {
    if (p.errorSummary() > 0)
    {
      std::cout << "Invalid event n. " <<  evt  << " of " << p;
      return;
    }
    int nPixel = p.numPixels(evt);
    event::Adc_vec_t curHitMap;
    curHitMap.resize(nPixel);
    unsigned int x = 1000; //initialize to non-physical value
    unsigned int y = 1000; //initialize to non-physical value
    adc_count_t height = 0;
    for (int index = 0; index < nPixel; ++index)
    {
      p.readPixel(evt, index, x, y, height);
      curHitMap.at(index) = height;
      //zero suppression in the hit map
      if (height > m_preferences.m_zeroSuppressionThreshold)
        m_hitMap->fill(x, y, static_cast<double> (height));
      else
        m_hitMap->fill(x, y, 0.);      
    }
    pEvent tmpEvt = pEvent(p.xmin(evt), p.xmax(evt), p.ymin(evt), p.ymax(evt),
			   curHitMap, p.microseconds(evt),
			   m_preferences.m_zeroSuppressionThreshold);
    tmpEvt.reconstruct(m_preferences.m_zeroSuppressionThreshold);
    emit eventRead();
    if (evtAccepted(tmpEvt)){
      m_isLastEventChanged = true;    
      m_lastEvent = tmpEvt;
      m_windowSizeHist->fill(nPixel);
      m_clusterSizeHist->fill(tmpEvt.clusterSize());
      m_pulseHeightHist->fill(tmpEvt.pulseHeight());
      m_modulationHist->fill(tmpEvt.phiDeg());
    }
  }
  // Here we release the memory. Using the data block
  // after this point will lead to incorect behaviour.
  delete [] data; 
}


bool pEventReader::evtAccepted(const pEvent& evt)
{
  return (evt.clusterSize() > m_preferences.m_minClusterSize &&
          evt.clusterSize() < m_preferences.m_maxClusterSize &&
          evt.pulseHeight() > m_preferences.m_minPulseHeight &&
          evt.pulseHeight() < m_preferences.m_maxPulseHeight &&
          evt.moma1().elongation() > m_preferences.m_minElongation &&
          evt.moma1().elongation() < m_preferences.m_maxElongation &&
          evt.evtSize() > m_preferences.m_minWindowSize &&
          evt.evtSize() < m_preferences.m_maxWindowSize);
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
