#ifndef PEVENTREADER_H
#define PEVENTREADER_H

#include <QThread>
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
    pEventReader(int socketPortNumber);
  
  public slots:
    void startReading();
    void readPendingDatagrams();
    void setStopped();
  
  signals:
    void pulseHeightRead(int pulseHeight);
    void barycenterRead(double xBar, double yBar);
  
  private:
    int m_socketPortNumber;
    bool m_stopped;
    QThread thread;
    QMutex m_mutex;
    QUdpSocket m_udpSocket;
    
    void readPendingDatagram();
    pDataBlock readDataBlock(QByteArray const &datagram);
    
};

#endif //PEVENTREADER_H

