#include "pDisplayWindow.h"
#include "pMapOptions.h"
#include <ctime>
#include <cstdlib>

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
  /* Generate some fake data. This is only for testing and should be removed
  as soon as we are able to generate sensible input values */

  const int nPedestals = 105600;
  double baseline = 1000.;
  double fluctuation_range = 500.;
  std::vector<double> pedestalValues (nPedestals);
  std::vector<double> pedestalRMS (nPedestals);
  
  //initialize random generator
  std::srand(static_cast<unsigned int>(time(0)));
  static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0); 
  std::rand();
  
  for (int pedIndex=0; pedIndex < nPedestals; pedIndex++)
  {
    double random_fluctuation = static_cast<double>(std::rand() * fraction 
                      * (fluctuation_range + 1)  - (0.5 * fluctuation_range));
    pedestalValues.at(pedIndex) = baseline + random_fluctuation;
    pedestalRMS.at(pedIndex) = random_fluctuation;
  }
  
  /*End of fake data generation*/
  
  int nx = 352;
  int ny = 300;
  double x_min = 1.;
  double x_max = static_cast<double>(nx);
  double y_min = 1.;
  double y_max = static_cast<double>(ny);
  
  pMapOptions *pedValOptions = new pMapOptions (x_min, x_max, y_min, y_max,
                                                nx, ny, "x", "y",
                                                "Pedestal values");
  pMapOptions *pedRmsOptions = new pMapOptions (x_min, x_max, y_min, y_max,
                                                nx, ny, "x", "y", "RMS");
  
  m_pedestalMap = new PedestalsMap(m_pedestalPlot, pedestalValues,
                                                     pedValOptions);
  m_pedestalRmsMap = new PedestalsMap(m_pedestalRmsPlot, pedestalRMS,
                                                        pedRmsOptions);
}
