#ifndef PEVENTREADER_H
#define PEVENTREADER_H

#include <vector>

#include <QMutex>
#include <QUdpSocket>
#include <QHostAddress>

#include "xpemonPlotOptions.h"
#include "pDataBlock.h"
#include "pHistogram.h"
#include "pMap.h"

class pEventReader: public QObject
{

  Q_OBJECT
  
  public:
  
    pEventReader(unsigned int socketPortNumber, double zeroSupThreshold);
  
  public slots:
  
    void startReading();
    void readPendingDatagrams();
    void updateRequested();
    void setStopped();
    void setSocketPortNumber(unsigned int socketPortNumber);
    void setZeroSupThreshold(double zeroSupThreshold);
    void resetHistograms();
  
  signals:
  
    void stopped();
    void eventRead(unsigned int xmin, unsigned int xmax,
                   unsigned int ymin, unsigned int ymax);
    void highestPixelFound(unsigned int highestX, unsigned int highestY);
    void pulseHeightUpdated(const std::vector<double>& pulseHeightValues);
    void windowSizeUpdated(const std::vector<double>& windowSizeValues);
    void hitMapUpdated(const std::vector<double>& hitMapValues);
    void evtDisplayUpdated(double xmin, double xmax, double ymin, double ymax,
                           const std::vector<double>& displayValues);
  
  private:
    
    void readPendingDatagram();
  
    pHistogram *m_pulseHeightHist;
    pHistogram *m_windowSizeHist;
    pMap *m_hitMap;
    //Current event info:
    unsigned int m_curXmin;
    unsigned int m_curXmax;
    unsigned int m_curYmin;
    unsigned int m_curYmax;
    std::vector<double> m_curHitMap;
      
    unsigned int m_socketPortNumber;
    double m_zeroSupThreshold;
    bool m_stopped;
    bool m_isContentChanged;
    QMutex m_mutex;
    QUdpSocket* m_udpSocket;
    
};

#endif //PEVENTREADER_H

