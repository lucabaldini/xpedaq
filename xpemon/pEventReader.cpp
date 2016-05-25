#include "pEventReader.h"

pEventReader::pEventReader(unsigned int socketPortNumber,
                           double zeroSupThreshold) : 
                           m_socketPortNumber(socketPortNumber),
                           m_zeroSupThreshold(zeroSupThreshold)
{
}


void pEventReader::readPendingDatagram()
{
  std::cout << "data received" << std::endl;
  char* data = new char[m_udpSocket.pendingDatagramSize()];
  QHostAddress sender;
  quint16 senderPort;
  m_udpSocket.readDatagram(data, m_udpSocket.pendingDatagramSize(),
                             &sender, &senderPort);
  pDataBlock p (reinterpret_cast<unsigned char*> (data));
  std::cout << "created datablock: " << p << std::endl;
  std::vector<pEvent> evtVec = p.events();
  for(std::vector<pEvent>::iterator evt = evtVec.begin();
      evt != evtVec.end();
      ++evt)
  {
    std::cout << "reading event: " << (*evt);
    /*std::vector<int> pulseHeights = (*evt).pulseHeightsOverThreshold(10); 
    for(std::vector<int>::iterator it = pulseHeights.begin();
      it != pulseHeights.end();
      ++it)
    {
      emit pulseHeightRead(*it);
    } */
    int pulseHeight = (*evt).totPulseHeightsOverThreshold(m_zeroSupThreshold);
    emit pulseHeightRead(pulseHeight);
    double xBar;
    double yBar;
    (*evt).barycenter(xBar, yBar);
    emit barycenterRead(xBar, yBar);
  }

  //debug
  //int counter;
  //QDataStream in(&datagram, QIODevice::ReadOnly);
  //in.setVersion(QDataStream::Qt_4_3);
  //in >> counter;
  //emit pulseHeightRead(counter);
}

void pEventReader::readPendingDatagrams()
{  
  QMutexLocker locker(&m_mutex);
  while ((!m_stopped) && (m_udpSocket.hasPendingDatagrams()))
  {
    readPendingDatagram();
  }
}


void pEventReader::startReading()
{
  std::cout << "Reading data" << std::endl;
  m_stopped = false;
  m_udpSocket.bind(m_socketPortNumber);
  connect(&m_udpSocket, SIGNAL(readyRead()), 
          this, SLOT(readPendingDatagrams()));
}


void pEventReader::setStopped()
{
  QMutexLocker locker(&m_mutex);
  m_udpSocket.disconnectFromHost();
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
