#include "pEventReader.h"

pEventReader::pEventReader(unsigned int socketPortNumber,
                           double zeroSupThreshold) : 
                           m_socketPortNumber(socketPortNumber),
                           m_zeroSupThreshold(zeroSupThreshold)
{
  m_udpSocket = new QUdpSocket (this);
}


void pEventReader::readPendingDatagram()
{
  //std::cout << "data received" << std::endl;
  qint64 size = m_udpSocket -> pendingDatagramSize();
  //we need a char* because QUdpSocket->readDatagram works that way  
  char* data = new (std::nothrow) char[size];
  if (data == nullptr)
  {
    std::cout << "allocation failed" << std::endl;
    return;
  }
  m_udpSocket -> readDatagram(data, size);
  /* When instantiating a pDataBlock we are really just passing to it a pointer
     to the buffer - no actual copy of the data involved.
     Here we do not create the pDataBlock dinamycally, so that it goes out
     of scope at the end of the function and the buffer gets deleted from memory
     (see the destructor of pDataBlock).
     Note: the cast on the pointer passed as argument does not modify the
     content of the buffer.
  */

  pDataBlock p (reinterpret_cast<unsigned char*> (data));
  for (unsigned int evt = 0; evt < p.numEvents(); ++evt)
  {
    unsigned int xmin = p.xmin(evt);
    unsigned int xmax = p.xmax(evt);
    unsigned int ymin = p.ymin(evt);    
    unsigned int ymax = p.ymax(evt);
    if ((xmax <= xmin) || (ymax <= ymin))
    {
      std::cout << "Invalid event of index " <<  evt  << " in " << p;
      continue;
    }    
    emit eventRead (xmin, xmax, ymin, ymax);
    unsigned int nPixel = p.numPixels(evt);
    unsigned int nCol = xmax - xmin +1;
    unsigned int adcSum = 0;
    unsigned int highestX = 0;
    unsigned int highestY = 0;
    unsigned int maxVal = 0;
    for (unsigned int index = 0; index < nPixel; index += 1)
    {
      unsigned int height = p.pixelCounts(evt, index);
      if (height < m_zeroSupThreshold) continue;
      unsigned int x = xmin + index % nCol;
      unsigned int y = ymin + index / nCol;
      emit pulseHeightRead(x, y, height);
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
      emit totPulseHeightRead(adcSum);
      emit highestPixelFound(highestX, highestY);
      //xBarycenter /= adcSum;
      //yBaricenter /= adcSum;
      //emit barycenterRead(xBarycenter, yBaricenter);      
    }
  }
  data = NULL; //will point to garbage when the pDataBlock goes out of scope
}  


void pEventReader::readPendingDatagrams()
{  
  QMutexLocker locker(&m_mutex);
  while ((!m_stopped) && (m_udpSocket -> hasPendingDatagrams()))
  {
    readPendingDatagram();
  }
}


void pEventReader::startReading()
{
  std::cout << "Reading data" << std::endl;
  QMutexLocker locker(&m_mutex);
  m_stopped = false;
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
