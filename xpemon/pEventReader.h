#ifndef PEVENTREADER_H
#define PEVENTREADER_H

#include <QMutex>
#include <QUdpSocket>
#include <QHostAddress>

#include "pDataBlock.h"
#include "pEvent.h"

class pEventReader: public QObject
{

  Q_OBJECT
  
  public:
  
    pEventReader(unsigned int socketPortNumber, double zeroSupThreshold);
  
  public slots:
  
    void startReading();
    void readPendingDatagrams();
    void setStopped();
    void setSocketPortNumber(unsigned int socketPortNumber);
    void setZeroSupThreshold(double zeroSupThreshold);
  
  signals:
  
    void stopped();
    void eventRead(unsigned int xmin, unsigned int xmax,
                   unsigned int ymin, unsigned int ymax);    
    void pulseHeightRead(unsigned int x, unsigned int y,
                         unsigned int pulseHeight);
    void totPulseHeightRead(unsigned int totPulseHeight);
    void highestPixelFound(unsigned int highestX, unsigned int highestY);
    //void barycenterRead(double xBar, double yBar);
  
  private:
    
    void readPendingDatagram();
  
    unsigned int m_socketPortNumber;
    double m_zeroSupThreshold;
    bool m_stopped;
    QMutex m_mutex;
    QUdpSocket* m_udpSocket;
    
};

#endif //PEVENTREADER_H

