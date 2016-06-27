#include "pDisplayWindow.h"
#include <ctime>
#include <cstdlib>
#include <random> 

pDisplayWindow::pDisplayWindow(QWidget *parent, int posx, int posy,
                               int windowHeight, int windowWidth) :
                               QMainWindow(parent),
                               m_posx (posx), m_posy(posy),
                               m_windowHeight (windowHeight),
                               m_windowWidth (windowWidth)
{
  if (objectName().isEmpty())
    setObjectName(QString::fromUtf8("DisplayWindow"));
  resize(m_windowHeight, m_windowWidth);
  m_centralWidget = new QWidget(this);
  m_centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
  m_verticalLayout = new QVBoxLayout(m_centralWidget);
  m_verticalLayout->setSpacing(6);
  m_verticalLayout->setContentsMargins(11, 11, 11, 11);
  m_verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
  
  m_meanPlot = new pMapPlot(pedestals::kNx, 0, pedestals::kNx,
                            pedestals::kNy, 0, pedestals::kNy);
  m_meanPlot -> setObjectName(QString::fromUtf8("Pedestals Mean"));
  m_verticalLayout->addWidget(m_meanPlot);
    
  m_rmsPlot = new pMapPlot(pedestals::kNx, 0, pedestals::kNx,
                           pedestals::kNy, 0, pedestals::kNy);
  m_rmsPlot -> setObjectName(QString::fromUtf8("Pedestals RMS"));
  m_verticalLayout->addWidget(m_rmsPlot);

  setCentralWidget(m_centralWidget);
  m_statusBar = new QStatusBar(this);
  m_statusBar->setObjectName(QString::fromUtf8("statusBar"));
  setStatusBar(m_statusBar);
  
  setGeometry(m_posx, m_posy, m_windowHeight, m_windowWidth);
}


/* Display the results (average and RMS) of a pedestal measurement
 */
void pDisplayWindow::showPedestals(const PedestalsMap& pedMap)
{
  using namespace pedestals; 
  
  for (unsigned int xIndex=0; xIndex < pedestals::kNx; xIndex++)
  {
    for (unsigned int yIndex=0; yIndex < pedestals::kNy; yIndex++)
    {
      try
      {
        m_meanPlot -> fillBin (xIndex, yIndex, pedMap.average (xIndex,yIndex));
        m_rmsPlot -> fillBin (xIndex, yIndex, pedMap.rms (xIndex,yIndex));
      }
      catch (int err)
      {
        std::cout << "WARNING: empty pixel" << std::endl;
        continue;
      }    
    }
  }
  m_meanPlot -> replot();
  m_rmsPlot -> replot();
}
