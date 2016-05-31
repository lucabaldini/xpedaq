#ifndef PEVENTREADER_H
#define PEVENTREADER_H

#include <QMutex>
#include <QUdpSocket>
#include <QByteArray>
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
    void evtEventRead(unsigned int xmin, unsigned int xmax,
                      unsigned int ymin, unsigned int ymax);    
    void pulseHeightRead(double x, double y, unsigned int pulseHeight);
    void totPulseHeightRead(unsigned int totPulseHeight);
    void barycenterRead(double xBar, double yBar);
    void highestPixelFound(unsigned int highestX, unsigned int highestY);
  
  private:
    unsigned int m_socketPortNumber;
    double m_zeroSupThreshold;
    bool m_stopped;
    QMutex m_mutex;
    QUdpSocket* m_udpSocket;

    void readPendingDatagram();
    pDataBlock readDataBlock(QByteArray const &datagram);
    
};

#endif //PEVENTREADER_H

