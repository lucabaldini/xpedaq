#include "pedviewerPlotGrid.h"

pedviewerPlotGrid::pedviewerPlotGrid(QWidget *parent) : QWidget(parent)
{
  m_PlotLayout = new QGridLayout(this);
  // Get as much space as possible, starting from the preferred
  // initial size
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}


void pedviewerPlotGrid::setupAverageMap()
{
  pColorMapOptions averageMapOptions ("Average map", "x", "y",
                                      "average [counts]",
                                      QCPColorGradient::gpThermal);
  m_averageMap = new pMap(pedestals::kNcol, 0, pedestals::kNcol,
                          pedestals::kNrow, 0, pedestals::kNrow);
  m_averageMapPlot = new pMapPlot(m_averageMap, averageMapOptions);
  m_averageMapPlot -> setObjectName(QString::fromUtf8("average map"));                              
  m_PlotLayout -> addWidget(m_averageMapPlot, 0, 0);
}


void pedviewerPlotGrid::setupAverageHist(double xmin, double xmax)
{
  pBasicPlotOptions averageHistOptions = pBasicPlotOptions("Average",
                                     "Average [counts]", "n. pixel");
  
  m_averageHist = new pHistogram(200, xmin, xmax);
  m_averagePlot = new pHistogramPlot(m_averageHist, averageHistOptions);
  m_averagePlot -> setObjectName(QString::fromUtf8("average hist"));
  m_PlotLayout -> addWidget(m_averagePlot, 0, 1);
}


void pedviewerPlotGrid::setupRmsMap()
{
  pColorMapOptions rmsMapOptions ("Rms map", "x", "y", "rms [counts]",
                                   QCPColorGradient::gpThermal);
  m_rmsMap = new pMap(pedestals::kNcol, 0, pedestals::kNcol,
                      pedestals::kNrow, 0, pedestals::kNrow);
  m_rmsMapPlot = new pMapPlot(m_rmsMap, rmsMapOptions);
  m_rmsMapPlot -> setObjectName(QString::fromUtf8("rms map"));
  m_PlotLayout -> addWidget(m_rmsMapPlot, 1, 0);
}


void pedviewerPlotGrid::setupRmsHist(double xmin, double xmax)
{
  pBasicPlotOptions rmsHistOptions = pBasicPlotOptions("Rms",
                                        "Rms [counts]", "n. pixel");
  m_rmsHist = new pHistogram(100, xmin, xmax);
  m_rmsPlot = new pHistogramPlot(m_rmsHist, rmsHistOptions);
  m_rmsPlot -> setObjectName(QString::fromUtf8("rms hist"));
  m_PlotLayout -> addWidget(m_rmsPlot, 1, 1);
}


void pedviewerPlotGrid::fillPlots(const PedestalsMap& pedMap)
{
  double averageMax = 100.;
  double rmsMax = 100.;
  for (unsigned int col=0; col < pedestals::kNcol; ++col){
    for (unsigned int row=0; row < pedestals::kNrow; ++row){
      double average = pedMap.average (col,row);
      double rms;
      if (average > averageMax) averageMax = average;      
      try {
        rms = pedMap.rms (col,row);
      }
      catch (int err) {
        rms = 0;
      }
      if (rms > rmsMax) rmsMax = rms;
    }
  }
  setupAverageMap();
  setupAverageHist(0., 1.5*averageMax);
  setupRmsMap();
  setupRmsHist(0., 1.5*rmsMax);
  for (unsigned int col=0; col < pedestals::kNcol; ++col){
    for (unsigned int row=0; row < pedestals::kNrow; ++row){
      double average = pedMap.average (col,row);
      double rms= pedMap.rms (col,row);
      m_averageMap -> fillBin (col, row, average);
      m_rmsMap -> fillBin (col, row, rms);                            
      m_averageHist -> fill (average);
      m_rmsHist -> fill (rms);        
    }
  }
  m_averagePlot -> updateDisplay();
  m_rmsPlot -> updateDisplay();
  m_averageMapPlot -> updateDisplay();
  m_rmsMapPlot -> updateDisplay();
}


void pedviewerPlotGrid::replotAll()
{
  m_averageMapPlot -> replot();
  m_averagePlot -> replot();
  m_rmsMapPlot -> replot();
  m_rmsPlot -> replot();
}
