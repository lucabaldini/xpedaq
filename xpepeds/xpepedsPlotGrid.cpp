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
  m_averageMap = new pMapPlot(pedestals::kNx, 0, pedestals::kNx,
                              pedestals::kNy, 0, pedestals::kNy,
                              averageMapOptions);
  m_averageMap -> setObjectName(QString::fromUtf8("average map"));                              
  m_PlotLayout -> addWidget(m_averageMap, 0, 0);
}


void xpepedsPlotGrid::setupAverageHist()
{
  pBasicPlotOptions averageHistOptions = pBasicPlotOptions("Average",
                                     "Average [counts]", "n. pixel");
  m_averageHist = new pHistogramPlot(200, 0., 1800.,
                                     averageHistOptions);
  m_averageHist -> setObjectName(QString::fromUtf8("average hist"));
  m_PlotLayout -> addWidget(m_averageHist, 0, 1);
}


void xpepedsPlotGrid::setupRmsMap()
{
  pColorMapOptions rmsMapOptions ("Rms map", "x", "y", "rms [counts]",
                                   QCPColorGradient::gpThermal);
  m_rmsMap = new pMapPlot(pedestals::kNx, 0, pedestals::kNx,
                          pedestals::kNy, 0, pedestals::kNy,
                          rmsMapOptions);
  m_rmsMap -> setObjectName(QString::fromUtf8("rms map"));
  m_PlotLayout -> addWidget(m_rmsMap, 1, 0);
}


void xpepedsPlotGrid::setupRmsHist()
{
  pBasicPlotOptions rmsHistOptions = pBasicPlotOptions("Rms",
                                        "Rms [counts]", "n. pixel");
  m_rmsHist = new pHistogramPlot(100, 0., 150., rmsHistOptions);
  m_rmsHist -> setObjectName(QString::fromUtf8("rms hist"));
  m_PlotLayout -> addWidget(m_rmsHist, 1, 1);
}


void xpepedsPlotGrid::fillPlots(const PedestalsMap& pedMap)
{
  for (unsigned int xIndex=0; xIndex < pedestals::kNx; xIndex++){
    for (unsigned int yIndex=0; yIndex < pedestals::kNy; yIndex++){
      try {
        double average = pedMap.average (xIndex,yIndex);
        double rms = pedMap.rms (xIndex,yIndex);
        //std::cout <<  "x = " << xIndex << ", y = " << yIndex << ", average = " << average << std::endl;
        m_averageMap -> fillBin (xIndex, yIndex, average);
        m_rmsMap -> fillBin (xIndex, yIndex, rms);                            
        m_averageHist -> fill (average);
        m_rmsHist -> fill (rms);
        
      }
      catch (int err){
        continue;
      }    
    }
  }
}


void xpepedsPlotGrid::replotAll()
{
  m_averageMap -> replot();
  m_averageHist -> replot();
  m_rmsMap -> replot();
  m_rmsHist -> replot();
}
