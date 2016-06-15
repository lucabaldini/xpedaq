#ifndef PMONITOROPTIONS_H
#define PMONITOROPTIONS_H

class pMonitorOptions
{

  public:
  
    pMonitorOptions(double refreshInterval = 500.,
                    unsigned int socketPortNumber = 50001,
                    unsigned int zeroSupThreshold = 1):
                    m_refreshInterval (refreshInterval),
                    m_socketPortNumber(socketPortNumber),
                    m_zeroSupThreshold(zeroSupThreshold) {;}
  
    double m_refreshInterval; //milliseconds
    unsigned int m_socketPortNumber;
    unsigned int m_zeroSupThreshold;
};

#endif //PMONITOROPTIONS_H
