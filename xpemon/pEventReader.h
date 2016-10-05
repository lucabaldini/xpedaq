#ifndef PEVENTREADER_H
#define PEVENTREADER_H

#include <vector>

#include <QMutex>
#include <QUdpSocket>
#include <QHostAddress>

#include "pDataBlock.h"
#include "pHistogram.h"
#include "pMap.h"
#include "pEvent.h"
#include "pCluster.h"

class pEventReader: public QObject
{

  Q_OBJECT
  
  public:
  
    pEventReader(unsigned int socketPortNumber, double zeroSupThreshold,
                 pHistogram* pulseHeightHist, pHistogram* windowSizeHist,
                 pMap* hitMap);
  
  public slots:
  
    void startReading();
    void readPendingDatagrams();
    void updateRequested();
    void setStopped();
    void setSocketPortNumber(unsigned int socketPortNumber);
    void setZeroSupThreshold(double zeroSupThreshold);
  
  signals:
  
    void stopped();
    void eventRead(unsigned int xmin, unsigned int xmax,
                   unsigned int ymin, unsigned int ymax);
    void highestPixelFound(unsigned int highestX, unsigned int highestY);
    void barycenterRead(double xBarycenter, double yBarycenter);
    
    void pulseHeightUpdated();
    void windowSizeUpdated();
    void hitMapUpdated();
    void evtDisplayUpdated(const pEvent& evt);
  
  private:
    
    void readPendingDatagram();
  
    //Data structures
    pHistogram *m_pulseHeightHist;
    pHistogram *m_windowSizeHist;
    pMap *m_hitMap;
    
    //Current event info:
    pEvent m_lastEvent;
      
    unsigned int m_socketPortNumber;
    double m_zeroSupThreshold;
    bool m_stopped;
    bool m_isContentChanged;
    QMutex m_mutex;
    QUdpSocket* m_udpSocket;
    
};

#endif //PEVENTREADER_H

