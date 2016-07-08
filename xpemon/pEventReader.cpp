#include "pEventReader.h"

using namespace xpemonPlotOptions;

pEventReader::pEventReader(unsigned int socketPortNumber,
                           double zeroSupThreshold) : 
                           m_curXmin(0), m_curXmax(0),
                           m_curYmin(0), m_curYmax(0),
                           m_socketPortNumber(socketPortNumber),
                           m_zeroSupThreshold(zeroSupThreshold)
{
  m_udpSocket = new QUdpSocket (this);
  m_pulseHeightHist = new pHistogram(pulseHeightNbins, pulseHeightXmin,
                                     pulseHeightXmax);
  m_windowSizeHist = new pHistogram(windowSizeNbins, windowSizeXmin,
                                    windowSizeXmax);
  //We shift the edges so that bins are centered on pixel indices
  m_hitMap = new pMap(xpoldetector::kNumPixelsX, -0.5, xPixelMax - 0.5,
                      xpoldetector::kNumPixelsY, -0.5, yPixelMax -0.5);
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
    if ((p.xmax(evt) <= p.xmin(evt)) || (p.ymax(evt) <= p.ymin(evt)))
    {
      std::cout << "Invalid event n. " <<  evt  << " of " << p;
      return;
    }
    m_isContentChanged = true;    
    m_curXmin = p.xmin(evt);
    m_curXmax = p.xmax(evt);
    m_curYmin = p.ymin(evt);    
    m_curYmax = p.ymax(evt);
    emit eventRead(m_curXmin, m_curXmax, m_curYmin, m_curYmax);
    unsigned int nPixel = p.numPixels(evt);
    m_windowSizeHist -> fill(static_cast<double> (nPixel));
    m_curHitMap.resize(nPixel);
    unsigned int nCol = m_curXmax - m_curXmin +1;
    double adcSum = 0;
    unsigned int highestX = 0;
    unsigned int highestY = 0;
    double maxVal = 0;
    for (unsigned int index = 0; index < nPixel; ++index)
    {
      double height = p.pixelCounts(evt, index);
	  if (height < m_zeroSupThreshold) { height = 0.;}
      m_curHitMap.at(index) = height;
      unsigned int x = m_curXmin + index % nCol;
      unsigned int y = m_curYmin + index / nCol;
      m_hitMap -> fill(x, y, height);
      adcSum += height;
      if (height > maxVal)
      {
        highestX = x;
        highestY = y;
        maxVal = height;
      }
      //xBarycenter += height * x;
      //yBaricenter += height * y;
    }
    if (adcSum > 0)
    {
      m_pulseHeightHist -> fill(adcSum);
      emit highestPixelFound(highestX, highestY);
      //xBarycenter /= adcSum;
      //yBaricenter /= adcSum;  
    }
  }
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
  std::vector<double> pulseHeightValues = m_pulseHeightHist -> values();
  std::vector<double> windowSizeValues = m_windowSizeHist -> values();
  std::vector<double> hitMapValues = m_hitMap -> values();
  emit pulseHeightUpdated(pulseHeightValues);
  emit windowSizeUpdated(windowSizeValues);
  emit hitMapUpdated(hitMapValues);
  emit evtDisplayUpdated(m_curXmin, m_curXmax, m_curYmin, m_curYmax,
                         m_curHitMap);
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


void pEventReader::resetHistograms()
{
  /* We don't need a mutex here, since this can only happen when the acquisition
     is stopped */
  m_pulseHeightHist -> reset();
  m_windowSizeHist -> reset();
  m_hitMap -> reset();
}
