#include "xpepedsPlotGrid.h"

xpepedsPlotGrid::xpepedsPlotGrid(QWidget *parent) : QWidget(parent)
{
  m_PlotLayout = new QGridLayout(this);

  // Get as much space as possible, starting from the preferred
  // initial size
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  setupAverageMap();
  setupAverageHist();  
  setupRmsMap();
  setupRmsHist();
}


void xpepedsPlotGrid::setupAverageMap()
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


void xpepedsPlotGrid::setupAverageHist()
{
  pBasicPlotOptions averageHistOptions = pBasicPlotOptions("Average",
                                     "Average [counts]", "n. pixel");
  
  m_averageHist = new pHistogram(200, 0., 1800.);
  m_averagePlot = new pHistogramPlot(m_averageHist, averageHistOptions);
  m_averagePlot -> setObjectName(QString::fromUtf8("average hist"));
  m_PlotLayout -> addWidget(m_averagePlot, 0, 1);
}


void xpepedsPlotGrid::setupRmsMap()
{
  pColorMapOptions rmsMapOptions ("Rms map", "x", "y", "rms [counts]",
                                   QCPColorGradient::gpThermal);
  m_rmsMap = new pMap(pedestals::kNcol, 0, pedestals::kNcol,
                      pedestals::kNrow, 0, pedestals::kNrow);
  m_rmsMapPlot = new pMapPlot(m_rmsMap, rmsMapOptions);
  m_rmsMapPlot -> setObjectName(QString::fromUtf8("rms map"));
  m_PlotLayout -> addWidget(m_rmsMapPlot, 1, 0);
}


void xpepedsPlotGrid::setupRmsHist()
{
  pBasicPlotOptions rmsHistOptions = pBasicPlotOptions("Rms",
                                        "Rms [counts]", "n. pixel");
  m_rmsHist = new pHistogram(100, 0., 150.);
  m_rmsPlot = new pHistogramPlot(m_rmsHist, rmsHistOptions);
  m_rmsPlot -> setObjectName(QString::fromUtf8("rms hist"));
  m_PlotLayout -> addWidget(m_rmsPlot, 1, 1);
}


void xpepedsPlotGrid::fillPlots(const PedestalsMap& pedMap)
{
  for (unsigned int col=0; col < pedestals::kNcol; ++col){
    for (unsigned int row=0; row < pedestals::kNrow; ++row){
      try {
        double average = pedMap.average (col,row);
        double rms = pedMap.rms (col,row);
        m_averageMap -> fillBin (col, row, average);
        m_rmsMap -> fillBin (col, row, rms);                            
        m_averageHist -> fill (average);
        m_rmsHist -> fill (rms);
        
      }
      catch (int err){
        continue;
      }    
    }
  }
  m_averagePlot -> updateDisplay();
  m_rmsPlot -> updateDisplay();
  m_averageMapPlot -> updateDisplay();
  m_rmsMapPlot -> updateDisplay();
}


void xpepedsPlotGrid::replotAll()
{
  m_averageMapPlot -> replot();
  m_averagePlot -> replot();
  m_rmsMapPlot -> replot();
  m_rmsPlot -> replot();
}
