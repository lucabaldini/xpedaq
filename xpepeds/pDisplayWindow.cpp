#include "pDisplayWindow.h"

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
  m_centralWidget -> setObjectName(QString::fromUtf8("centralWidget"));
  m_verticalLayout = new QVBoxLayout(m_centralWidget);
  m_verticalLayout -> setSpacing(6);
  m_verticalLayout -> setContentsMargins(11, 11, 11, 11);
  m_verticalLayout -> setObjectName(QString::fromUtf8("verticalLayout"));

  m_plotGrid = new xpepedsPlotGrid();
  m_verticalLayout -> addWidget(m_plotGrid);

  setCentralWidget(m_centralWidget);
  m_statusBar = new QStatusBar(this);
  m_statusBar -> setObjectName(QString::fromUtf8("statusBar"));
  setStatusBar(m_statusBar);
  
  m_resetButton = new pQtCustomPushButton(m_centralWidget, "reset");
  m_verticalLayout -> addWidget(m_resetButton);
  
  setGeometry(m_posx, m_posy, m_windowHeight, m_windowWidth);
  connect (m_resetButton, SIGNAL(clicked()),
           m_plotGrid, SLOT(restorePlot()));
}


/* Display the results (average and RMS) of a pedestal measurement
 */
void pDisplayWindow::showPedestals(const PedestalsMap& pedMap)
{  
  m_plotGrid -> fillPlots(pedMap);
  m_plotGrid -> replotAll();
}


void pDisplayWindow::closeEvent(QCloseEvent *event)
{
  emit windowClosed();
  event->accept();
}
