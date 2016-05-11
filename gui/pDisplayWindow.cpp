#include "pDisplayWindow.h"
#include "pMapOptions.h"
#include "pedestalsMap.h"
#include <ctime>
#include <cstdlib>
#include <memory>

pDisplayWindow::pDisplayWindow(QWidget *parent, int posx, int posy,
  int windowHeight, int windowWidth) : QMainWindow(parent), m_posx (posx),
  m_posy(posy), m_windowHeight (windowHeight), m_windowWidth (windowWidth)
{
  if (this->objectName().isEmpty())
    this->setObjectName(QString::fromUtf8("DisplayWindow"));
    this->resize(m_windowHeight, m_windowWidth);
    m_centralWidget = new QWidget(this);
    m_centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
    m_verticalLayout = new QVBoxLayout(m_centralWidget);
    m_verticalLayout->setSpacing(6);
    m_verticalLayout->setContentsMargins(11, 11, 11, 11);
    m_verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    m_pedestalPlot = new QCustomPlot(m_centralWidget);
    m_pedestalPlot->setObjectName(QString::fromUtf8("pedestalPlot"));
    
    m_pedestalRmsPlot = new QCustomPlot(m_centralWidget);
    m_pedestalRmsPlot->setObjectName(QString::fromUtf8("pedestalRmsPlot"));
    
    m_verticalLayout->addWidget(m_pedestalPlot);
    m_verticalLayout->addWidget(m_pedestalRmsPlot);

    this->setCentralWidget(m_centralWidget);
    m_statusBar = new QStatusBar(this);
    m_statusBar->setObjectName(QString::fromUtf8("statusBar"));
    this->setStatusBar(m_statusBar);
  
  setGeometry(m_posx, m_posy, m_windowHeight, m_windowWidth);
  showPedestals();
}

/* Display the results (average and RMS) of a pedestal measurement
 */
void pDisplayWindow::showPedestals()
{
  using namespace pedestals;
  
  /* Generate some fake data. This is only for testing and should be removed
  as soon as we are able to generate sensible input values */

  double baseline = 1000.;
  double fluctuation_range = 500.;
  arr_t pedestalValues;
  arr_t pedestalRMS;
  
  //initialize random generator
  std::srand(static_cast<unsigned int>(time(0)));
  static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 
  std::rand();
  
  for (int pedIndex=0; pedIndex < kNPedestal; pedIndex++)
  {
    double random_fluctuation = static_cast<double>(std::rand() * fraction 
                      * (fluctuation_range + 1)  - (0.5 * fluctuation_range));
    pedestalValues.at(pedIndex) = baseline + random_fluctuation;
    pedestalRMS.at(pedIndex) = random_fluctuation;
  }
  
  /*End of fake data generation*/
  
  std::shared_ptr<pColorMapOptions> pedValOptions (new pColorMapOptions ("x",
                                                    "y", "Pedestal values"));
  std::shared_ptr<pColorMapOptions> pedRmsOptions (new pColorMapOptions ("x",
                                                    "y", "RMS"));  
  
  PedestalsMapData* val = new PedestalsMapData(pedestalValues);
  PedestalsMapData* rms = new PedestalsMapData(pedestalRMS);
  m_pedestalMap = new PedestalsMap(m_pedestalPlot, val, pedValOptions);
  m_pedestalRmsMap = new PedestalsMap(m_pedestalRmsPlot, rms, pedRmsOptions);
}
