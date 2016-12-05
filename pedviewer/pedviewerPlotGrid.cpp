#include "pedviewerPlotGrid.h"
#include "pQtCustomLineEdit.h"

pedviewerPlotGrid::pedviewerPlotGrid(QWidget *parent) : QWidget(parent)
{
  m_PlotLayout = new QGridLayout(this);
  // Get as much space as possible, starting from the preferred
  // initial size
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  m_prevButton = new QPushButton("Prev");
  m_nextButton = new QPushButton("Next");
  m_evtNumberEdit = new pQtCustomLineEdit<int>();
  QString initialLabel = QString("/ ") + QString::number(0);
  m_totEvtLabel = new pQtCustomTextLabel(this, initialLabel);

  QWidget* buttonWidget = new QWidget();
  m_navLayout = new QHBoxLayout(buttonWidget);
  m_navLayout->addWidget(m_prevButton);
  m_navLayout->addWidget(m_nextButton);
  m_navLayout->addWidget(m_evtNumberEdit);
  m_navLayout->addWidget(m_totEvtLabel);

  m_PlotLayout->addWidget(buttonWidget, 0, 0, 1, 2);
}


/*
*/
void pedviewerPlotGrid::fillPlots(const PedestalsMap& pedMap)
{
  double averageMax = 100.;
  double rmsMax = 100.;
  for (unsigned int col=0; col < xpoldetector::kNumPixelsX; ++col){
    for (unsigned int row=0; row < xpoldetector::kNumPixelsY; ++row){
      double average = pedMap.average (col,row);
      double rms = pedMap.rms (col,row);;
      if (average > averageMax) averageMax = average;      
      if (rms > rmsMax) rmsMax = rms;
    }
  }
  pColorMapOptions averageMapOptions ("Average map", "x", "y",
                                      "average [counts]",
                                      QCPColorGradient::gpThermal);
  setupSupMap(averageMapOptions);
  pBasicPlotOptions averageHistOptions = pBasicPlotOptions("Average",
                                         "Average [counts]", "n. pixel");
  setupSupHist(averageHistOptions, 0., 1.5*averageMax);
  pColorMapOptions rmsMapOptions ("Rms map", "x", "y", "rms [counts]",
                                  QCPColorGradient::gpThermal);
  setupInfMap(rmsMapOptions);
  pBasicPlotOptions rmsHistOptions = pBasicPlotOptions("Rms",
                                     "Rms [counts]", "n. pixel");
  setupInfHist(rmsHistOptions, 0., 1.5*rmsMax);
  for (unsigned int col=0; col < xpoldetector::kNumPixelsX; ++col){
    for (unsigned int row=0; row < xpoldetector::kNumPixelsY; ++row){
      double average = pedMap.average (col,row);
      double rms= pedMap.rms (col,row);
      m_supMap -> fillBin (col, row, average);
      m_infMap -> fillBin (col, row, rms);                            
      m_supHist -> fill (average);
      m_infHist -> fill (rms);        
    }
  }
  m_supPlot -> updateDisplay();
  m_infPlot -> updateDisplay();
  m_supMapPlot -> updateDisplay();
  m_infMapPlot -> updateDisplay();
}


/*
*/
void pedviewerPlotGrid::fillPlots(const PedestalsMap& pedMap,
                                  const PedestalsMap& refMap)
{
  double diffMax = 100.;
  double diffMin = -100.;
  double normDiffMax = 1.;
  double normDiffMin = -1.;
  for (unsigned int col=0; col < xpoldetector::kNumPixelsX; ++col){
    for (unsigned int row=0; row < xpoldetector::kNumPixelsY; ++row){
      double diff = pedMap.average (col,row) - refMap.average (col,row);
      double rms = pow(pow(pedMap.rms(col,row), 2.)
                       + pow(refMap.rms(col,row), 2.), 0.5);
      double normDiff = diff/rms;
      if (diff > diffMax) diffMax = diff;
      if (diff < diffMin) diffMin = diff;
      if (normDiff > normDiffMax) normDiffMax = normDiff;
      if (normDiff < normDiffMin) normDiffMin = normDiff;
    }
  }
  pColorMapOptions averageMapOptions ("Difference map", "x", "y",
                                      "average - reference [counts]",
                                      QCPColorGradient::gpThermal);
  setupSupMap(averageMapOptions);
  pBasicPlotOptions averageHistOptions = pBasicPlotOptions("Difference",
                                  "average - reference [counts]", "n. pixel");
  setupSupHist(averageHistOptions, 1.5*diffMin, 1.5*diffMax);
  pColorMapOptions rmsMapOptions ("Normalized difference", "x", "y",
                                  "average - reference [sigma]",
                                  QCPColorGradient::gpThermal);
  setupInfMap(rmsMapOptions);
  pBasicPlotOptions rmsHistOptions = pBasicPlotOptions("Rms",
                                   "average - reference [sigma]", "n. pixel");
  setupInfHist(rmsHistOptions, 1.5*normDiffMin , 1.5*normDiffMax);
  for (unsigned int col=0; col < xpoldetector::kNumPixelsX; ++col){
    for (unsigned int row=0; row < xpoldetector::kNumPixelsY; ++row){
      double diff = pedMap.average (col,row) - refMap.average (col,row);
      double rms = pow(pow(pedMap.rms(col,row), 2.)
                       + pow(refMap.rms(col,row), 2.), 0.5);
      double normDiff = diff/rms;
      m_supMap -> fillBin (col, row, diff);
      m_infMap -> fillBin (col, row, normDiff);                            
      m_supHist -> fill (diff);
      m_infHist -> fill (normDiff);        
    }
  }
  m_supPlot -> updateDisplay();
  m_infPlot -> updateDisplay();
  m_supMapPlot -> updateDisplay();
  m_infMapPlot -> updateDisplay();
}


/*
*/
void pedviewerPlotGrid::replotAll()
{
  m_supMapPlot -> replot();
  m_supPlot -> replot();
  m_infMapPlot -> replot();
  m_infPlot -> replot();
}


/*
*/
void pedviewerPlotGrid::setNextButtonEnabled(bool enabled)
{
  m_nextButton->setEnabled(enabled);
} 


/*
*/
void pedviewerPlotGrid::setPrevButtonEnabled(bool enabled)
{
  m_prevButton->setEnabled(enabled);
}


/*
*/
void pedviewerPlotGrid::setButtonsEnabled(bool enabled)
{
  setPrevButtonEnabled(enabled);
  setNextButtonEnabled(enabled);
}


/*
*/
void pedviewerPlotGrid::setTotEvtLabel(int numEvents)
{
  QString label = QString("/ ") + QString::number(numEvents);
  m_totEvtLabel->setText(label);
}


/*
*/
void pedviewerPlotGrid::updateEvtNumberEdit(int curEvent)
{
  m_evtNumberEdit->setVal(curEvent);
}


/*
*/
void pedviewerPlotGrid::clear()
{
  delete m_supMapPlot;
  delete m_infMapPlot;
  delete m_supPlot;
  delete m_infPlot;
  delete m_supMap;
  delete m_infMap;
  delete m_supHist;
  delete m_infHist;  
}


/*
*/
void pedviewerPlotGrid::setupSupMap(const pColorMapOptions& options)
{
  m_supMap = new pMap(xpoldetector::kNumPixelsX,
                      0, xpoldetector::kNumPixelsX,
                      xpoldetector::kNumPixelsY,
                      0, xpoldetector::kNumPixelsY);
  m_supMapPlot = new pMapPlot(m_supMap, options);
  m_supMapPlot -> setObjectName(QString::fromUtf8("average map"));                              
  m_PlotLayout -> addWidget(m_supMapPlot, 1, 0, 3, 1);
}


/*
*/
void pedviewerPlotGrid::setupSupHist(const pBasicPlotOptions& options,
                                     double xmin, double xmax)
{ 
  m_supHist = new pHistogram(200, xmin, xmax);
  m_supPlot = new pHistogramPlot(m_supHist, options);
  m_supPlot -> setObjectName(QString::fromUtf8("average hist"));
  m_PlotLayout -> addWidget(m_supPlot, 1, 1, 3, 1);
}


/*
*/
void pedviewerPlotGrid::setupInfMap(const pColorMapOptions& options)
{
  m_infMap = new pMap(xpoldetector::kNumPixelsX,
                      0, xpoldetector::kNumPixelsX,
                      xpoldetector::kNumPixelsY,
                      0, xpoldetector::kNumPixelsY);
  m_infMapPlot = new pMapPlot(m_infMap, options);
  m_infMapPlot -> setObjectName(QString::fromUtf8("rms map"));
  m_PlotLayout -> addWidget(m_infMapPlot, 4, 0, 3, 1);
}


/*
*/
void pedviewerPlotGrid::setupInfHist(const pBasicPlotOptions& options,
                                     double xmin, double xmax)
{
  m_infHist = new pHistogram(100, xmin, xmax);
  m_infPlot = new pHistogramPlot(m_infHist, options);
  m_infPlot -> setObjectName(QString::fromUtf8("rms hist"));
  m_PlotLayout -> addWidget(m_infPlot, 4, 1, 3, 1);
}
