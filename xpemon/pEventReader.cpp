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
  char* data = new char[m_udpSocket -> pendingDatagramSize()];
  QHostAddress sender;
  quint16 senderPort;
  m_udpSocket -> readDatagram(data, m_udpSocket -> pendingDatagramSize(),
                             &sender, &senderPort);
  pDataBlock p (reinterpret_cast<unsigned char*> (data));
  //std::cout << "created datablock: " << p << std::endl;
  std::vector<pEvent> evtVec = p.events();
  for(std::vector<pEvent>::iterator evt = evtVec.begin();
      evt != evtVec.end();
      ++evt)
  {
    //std::cout << "reading event: " << (*evt) << std::endl;
    using namespace event;
    Adc_vec_t counts  = (*evt).adcCounts();
    unsigned int xmin = (*evt).xmin();
    unsigned int xmax = (*evt).xmax();
    unsigned int ymin = (*evt).ymin();    
    unsigned int ymax = (*evt).ymax();
    unsigned int nCol = xmax - xmin + 1;  
    
    //std::cout << xmin << " " << ymin << " " << nCol << std::endl;
    emit evtEventRead (xmin, xmax, ymin, ymax);
    unsigned int adcSum = 0;
    unsigned int highestX = 0;
    unsigned int highestY = 0;
    unsigned int maxVal = 0;
    //double xBarycenter = 0.;
    //double yBaricenter = 0.;    
    
    for(unsigned int index = 0; index != counts.size(); ++index)
    {
      unsigned int height = counts.at(index);
      unsigned int x = xmin + index % nCol;
      unsigned int y = ymin + index / nCol;
      if (height > maxVal)
      {
        highestX = x;
        highestY = y;
        maxVal = height;
      }
      //std::cout << "( " << x << " , " << y << " ) = " << height << std::endl;  
      emit pulseHeightRead((double) x, (double) y, height);
      if (height > m_zeroSupThreshold)
      {
        adcSum += height;
        //xBarycenter += height * x;
        //yBaricenter += height * y;
      }
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
