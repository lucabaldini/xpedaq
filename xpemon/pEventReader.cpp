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

pEventReader::pEventReader(unsigned int socketPortNumber,
                           double zeroSupThreshold,
                           pHistogram* pulseHeightHist, 
                           pHistogram* windowSizeHist,
                           pHistogram* modulationHist,
                           pMap* hitMap) :
                           m_pulseHeightHist(pulseHeightHist),
                           m_windowSizeHist(windowSizeHist),
                           m_modulationHist(modulationHist),
                           m_hitMap(hitMap),
                           m_socketPortNumber(socketPortNumber),
                           m_zeroSupThreshold(zeroSupThreshold)
{
  m_udpSocket = new QUdpSocket (this);
}


void pEventReader::readPendingDatagram()
{
  qint64 size = m_udpSocket -> pendingDatagramSize();
  //we need a char* because of QUdpSocket->readDatagram() signature  
  char* data = new (std::nothrow) char[size];
  if (data == nullptr)
  {
    std::cout << "allocation failed" << std::endl;
    return;
  }
  m_udpSocket -> readDatagram(data, size);
  
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
    m_isContentChanged = true;    
    emit eventRead(p.xmin(evt), p.xmax(evt), p.ymin(evt), p.ymax(evt));
    int nPixel = p.numPixels(evt);
    m_windowSizeHist -> fill(static_cast<double> (nPixel));
    event::Adc_vec_t curHitMap;
    curHitMap.resize(nPixel);
    double adcSum = 0;
    unsigned int highestX = 0;
    unsigned int highestY = 0;
    adc_count_t maxVal = 0; //counts in the highest pixels
    unsigned int x = 1000; //initialize to non-physical value
    unsigned int y = 1000; //initialize to non-physical value
    adc_count_t height = 0;
    double xBarycenter = 0.;
    double yBarycenter = 0.;
    for (int index = 0; index < nPixel; ++index)
    {
      p.readPixel(evt, index, x, y, height);
      if (height < m_zeroSupThreshold) { height = 0.;}
      curHitMap.at(index) = height;
      m_hitMap -> fill(x, y, static_cast<double> (height));
      adcSum += height;
      if (height > maxVal)
      {
        highestX = x;
        highestY = y;
        maxVal = height;
      }
      xBarycenter += height * x;
      yBarycenter += height * y;
    }
    if (adcSum > 0)
    {
      m_pulseHeightHist -> fill(adcSum);
      emit highestPixelFound(highestX, highestY);
      xBarycenter /= adcSum;
      yBarycenter /= adcSum;
      emit barycenterRead(xBarycenter, yBarycenter);
    }
    m_lastEvent = pEvent(p.xmin(evt), p.xmax(evt), p.ymin(evt), p.ymax(evt),
                         curHitMap);
    m_lastEvent.clusterize(m_zeroSupThreshold);
    m_lastEvent.doMomentsAnalysis();
  }
  //std::cout << cluster;
  // Here we release the memory. Using the data block
  // after this point will lead to incorect behaviour.
  delete [] data; 
}  


void pEventReader::readPendingDatagrams()
{  
  QMutexLocker locker(&m_mutex);
  while ((!m_stopped) && (m_udpSocket -> hasPendingDatagrams()))
  {
    readPendingDatagram();
  }
}


void pEventReader::updateRequested()
{
  QMutexLocker locker(&m_mutex);
  if (!m_isContentChanged) return;
  emit pulseHeightUpdated();
  emit windowSizeUpdated();
  emit hitMapUpdated();
  emit evtDisplayUpdated(m_lastEvent);
  m_isContentChanged = false;                         
}


void pEventReader::startReading()
{
  QMutexLocker locker(&m_mutex);
  m_stopped = false;
  m_isContentChanged = false;
  m_udpSocket -> bind(m_socketPortNumber);
  connect(m_udpSocket, SIGNAL(readyRead()), 
          this, SLOT(readPendingDatagrams()));
}


void pEventReader::setStopped()
{
  QMutexLocker locker(&m_mutex);
  m_udpSocket -> disconnectFromHost();
  m_stopped = true;
  emit stopped();
}


void pEventReader::setSocketPortNumber(unsigned int socketPortNumber)
{
  QMutexLocker locker(&m_mutex);
  m_socketPortNumber = socketPortNumber;
}


void pEventReader::setZeroSupThreshold(double zeroSupThreshold)
{
  QMutexLocker locker(&m_mutex);
  m_zeroSupThreshold = zeroSupThreshold;
}
