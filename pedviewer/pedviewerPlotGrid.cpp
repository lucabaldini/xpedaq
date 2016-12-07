#include "pedviewerPlotGrid.h"
#include "pQtCustomLineEdit.h"

PedviewerPlotGrid::PedviewerPlotGrid(QWidget *parent) :
  QWidget(parent),
  m_supMap(nullptr),
  m_infMap(nullptr),
  m_supHist(nullptr),
  m_infHist(nullptr),  
  m_supMapPlot(nullptr),
  m_infMapPlot(nullptr),
  m_supPlot(nullptr),
  m_infPlot(nullptr)
{
  m_PlotLayout = new QGridLayout(this);
  // Get as much space as possible, starting from the preferred
  // initial size
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

/*
*/
PedviewerPlotGrid::~PedviewerPlotGrid()
{
  // The Qt object are taken care of by their parents widgets, so we need
  // to release the memory only of the rest of the stuff
  delete m_supMap;
  delete m_infMap;
  delete m_supHist;
  delete m_infHist;
}


/*
*/
void PedviewerPlotGrid::fillPlots(const PedestalsMap& pedMap,
                                  displayMode plotType)
{
  double aveMin, aveMax, rmsMin, rmsMax;
  findEdges(pedMap, aveMin, aveMax, rmsMin, rmsMax);
  QString averageLabel, rmsLabel;
  if (plotType == singleEvtType){
    averageLabel = "pixel value [counts]";
    rmsLabel = "pixel rms [counts]";
  } else {
    averageLabel = "pixel average [counts]";
    rmsLabel = "pixel rms [counts]";
  }
  
  
  pColorMapOptions averageMapOptions ("Average map", "x", "y", averageLabel,
   QCPColorGradient::gpThermal);
  setupSupMap(averageMapOptions);
  
  pBasicPlotOptions averageHistOptions = pBasicPlotOptions("Average",
    averageLabel, "n. pixel");
  setupSupHist(averageHistOptions, 0.9*aveMin, 1.1*aveMax);
  
  pColorMapOptions rmsMapOptions ("Rms map", "x", "y", rmsLabel,
    QCPColorGradient::gpThermal);
  setupInfMap(rmsMapOptions);
  
  pBasicPlotOptions rmsHistOptions = pBasicPlotOptions("Rms", rmsLabel,
    "n. pixel");
  setupInfHist(rmsHistOptions, 0.9*rmsMin, 1.1*rmsMax);
  
  for (unsigned int col=0; col < xpoldetector::kNumPixelsX; ++col){
    for (unsigned int row=0; row < xpoldetector::kNumPixelsY; ++row){
      double average = pedMap.average (col,row);
      double rms= pedMap.rms (col,row);
      m_supMap->fillBin (col, row, average);
      m_infMap->fillBin (col, row, rms);                            
      m_supHist->fill (average);
      m_infHist->fill (rms);        
    }
  }
  m_supPlot->updateDisplay();
  m_infPlot->updateDisplay();
  m_supMapPlot->updateDisplay();
  m_infMapPlot->updateDisplay();
}


/*
*/
void PedviewerPlotGrid::fillPlots(const PedestalsMap& pedMap,
                                  const PedestalsMap& refMap,
                                  displayMode)
{
  double diffMin, diffMax, normDiffMin, normDiffMax;
  findEdges(pedMap, refMap, diffMin, diffMax, normDiffMin, normDiffMax);
  
  QString diffLabel = "difference [counts]";
  QString normDiffLabel = "normalized difference [sigma]";

  pColorMapOptions averageMapOptions ("Difference map", "x", "y",
    diffLabel, QCPColorGradient::gpThermal);
  setupSupMap(averageMapOptions);
  
  pBasicPlotOptions averageHistOptions = pBasicPlotOptions("Difference",
    diffLabel, "n. pixel");
  double binLow = diffMin - 0.1*fabs(diffMin);
  double binUp = diffMax + 0.1*fabs(diffMax);
  setupSupHist(averageHistOptions, binLow, binUp);
  
  pColorMapOptions rmsMapOptions ("Normalized difference", "x", "y",
    normDiffLabel, QCPColorGradient::gpThermal);
  setupInfMap(rmsMapOptions);
  
  pBasicPlotOptions rmsHistOptions = pBasicPlotOptions("Rms",
    normDiffLabel, "n. pixel");
  binLow = normDiffMin - 0.1*fabs(normDiffMin);
  binUp = normDiffMax + 0.1*fabs(normDiffMax);
  setupInfHist(rmsHistOptions, binLow, binUp);
  
  for (unsigned int col=0; col < xpoldetector::kNumPixelsX; ++col){
    for (unsigned int row=0; row < xpoldetector::kNumPixelsY; ++row){
      double diff = pedMap.average (col,row) - refMap.average (col,row);
      double rms = pow(pow(pedMap.rms(col,row), 2.)
                       + pow(refMap.rms(col,row), 2.), 0.5);
      double normDiff = diff/rms;
      m_supMap->fillBin (col, row, diff);
      m_infMap->fillBin (col, row, normDiff);                            
      m_supHist->fill (diff);
      m_infHist->fill (normDiff);        
    }
  }
  m_supPlot->updateDisplay();
  m_infPlot->updateDisplay();
  m_supMapPlot->updateDisplay();
  m_infMapPlot->updateDisplay();
}


/*
*/
void PedviewerPlotGrid::replotAll()
{
  m_supMapPlot->replot();
  m_supPlot->replot();
  m_infMapPlot->replot();
  m_infPlot->replot();
}


/*
*/
void PedviewerPlotGrid::clear()
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
void PedviewerPlotGrid::findEdges(const PedestalsMap& pedMap,
                                  double& aveMin, double& aveMax,
                                  double& rmsMin, double& rmsMax) const
{
  aveMin = 10000.;
  aveMax = -10000.;
  rmsMin = 10000.;
  rmsMax = -10000.;
  for (unsigned int col=0; col < xpoldetector::kNumPixelsX; ++col){
    for (unsigned int row=0; row < xpoldetector::kNumPixelsY; ++row){
      double average = pedMap.average (col,row);
      double rms = pedMap.rms (col,row);;
      if (average > aveMax) aveMax = average;      
      if (rms > rmsMax) rmsMax = rms;
      if (average < aveMin) aveMin = average;      
      if (rms < rmsMin) rmsMin = rms;
    }
  }
}


/*
*/
void PedviewerPlotGrid::findEdges(const PedestalsMap& pedMap,
                                  const PedestalsMap& refMap,
                                  double& diffMin, double& diffMax,
                                  double& normDiffMin, double& normDiffMax)
                                  const
{
  diffMin = 10000.;
  diffMax = -10000.;
  normDiffMin = 10000.;
  normDiffMax = -10000.;
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
}


/*
*/
void PedviewerPlotGrid::setupSupMap(const pColorMapOptions& options)
{
  m_supMap = new pMap(xpoldetector::kNumPixelsX,
                      0, xpoldetector::kNumPixelsX,
                      xpoldetector::kNumPixelsY,
                      0, xpoldetector::kNumPixelsY);
  m_supMapPlot = new pMapPlot(m_supMap, options);
  m_supMapPlot->setObjectName(QString::fromUtf8("average map"));                              
  m_PlotLayout->addWidget(m_supMapPlot, 1, 0, 5, 1);
}


/*
*/
void PedviewerPlotGrid::setupSupHist(const pBasicPlotOptions& options,
                                     double xmin, double xmax)
{ 
  m_supHist = new pHistogram(200, xmin, xmax);
  m_supPlot = new pHistogramPlot(m_supHist, options);
  m_supPlot->setObjectName(QString::fromUtf8("average hist"));
  m_PlotLayout->addWidget(m_supPlot, 1, 1, 5, 1);
}


/*
*/
void PedviewerPlotGrid::setupInfMap(const pColorMapOptions& options)
{
  m_infMap = new pMap(xpoldetector::kNumPixelsX,
                      0, xpoldetector::kNumPixelsX,
                      xpoldetector::kNumPixelsY,
                      0, xpoldetector::kNumPixelsY);
  m_infMapPlot = new pMapPlot(m_infMap, options);
  m_infMapPlot->setObjectName(QString::fromUtf8("rms map"));
  m_PlotLayout->addWidget(m_infMapPlot, 6, 0, 5, 1);
}


/*
*/
void PedviewerPlotGrid::setupInfHist(const pBasicPlotOptions& options,
                                     double xmin, double xmax)
{
  m_infHist = new pHistogram(100, xmin, xmax);
  m_infPlot = new pHistogramPlot(m_infHist, options);
  m_infPlot->setObjectName(QString::fromUtf8("rms hist"));
  m_PlotLayout->addWidget(m_infPlot, 6, 1, 5, 1);
}
