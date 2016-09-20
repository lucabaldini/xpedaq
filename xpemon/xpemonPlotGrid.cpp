#include "xpemonPlotGrid.h"

using namespace xpemonPlotOptions;

xpemonPlotGrid::xpemonPlotGrid(QWidget *parent) : QWidget(parent)
{
  m_PlotLayout = new QGridLayout(this);

  // Get as much space as possible, starting from the preferred initial size
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  setupPulseHeightPlot(); 
  setupWindowSizePlot();
  setupHitMap();
  setupEventDisplay();
}


void xpemonPlotGrid::setupPulseHeightPlot()
{
  pBasicPlotOptions pulseHeightOptions = pBasicPlotOptions("Acd sum",
                             "Total over threshold evt adc counts", "n. evt");
  m_pulseHeightHist = new pHistogram(pulseHeightNbins, pulseHeightXmin,
                                     pulseHeightXmax);
  m_pulseHeightPlot = new pHistogramPlot(m_pulseHeightHist,
                                         pulseHeightOptions);
  m_PlotLayout -> addWidget(m_pulseHeightPlot, 0, 0);
}


void xpemonPlotGrid::setupWindowSizePlot()
{
  pBasicPlotOptions windowSizeOptions = pBasicPlotOptions("Window size",
                                             "Window size (pixel)", "n. evt");
  m_windowSizeHist = new pHistogram(windowSizeNbins, windowSizeXmin,
                                        windowSizeXmax);
  m_windowSizePlot = new pHistogramPlot(m_windowSizeHist, windowSizeOptions);
  m_PlotLayout -> addWidget(m_windowSizePlot, 0, 1);
}


void xpemonPlotGrid::setupHitMap()
{
  pColorMapOptions hitMapOptions ("Hit map", "x", "y", "counts",
                                  QCPColorGradient::gpThermal);
  m_hitMap = new pMap(xpoldetector::kNumPixelsX, -0.5, xPixelMax - 0.5,
                      xpoldetector::kNumPixelsY, -0.5, yPixelMax - 0.5);
  m_hitMapPlot = new pMapPlot(m_hitMap, hitMapOptions);
  m_PlotLayout -> addWidget(m_hitMapPlot, 1, 0);
}


void xpemonPlotGrid::setupEventDisplay()
{
  pColorMapOptions eventDisplayOptions ("Event display", "x", "y", "counts",
                                        QCPColorGradient::gpThermal);
  m_eventDisplay = new pCustomColorMapPlot(eventDisplayOptions);
  m_PlotLayout->addWidget(m_eventDisplay, 1, 1);
}


void xpemonPlotGrid::updatePulseHeightPlot()
{
  m_pulseHeightPlot -> updateDisplay ();
  m_pulseHeightPlot -> replot();
}


void xpemonPlotGrid::updateWindowSizePlot()
{
  m_windowSizePlot -> updateDisplay();
  m_windowSizePlot -> replot();
}


void xpemonPlotGrid::updateHitMapPlot()
{
  m_hitMapPlot -> updateDisplay ();
  m_hitMapPlot -> replot();
}


void xpemonPlotGrid::updateEventDisplay(double xmin, double xmax,
                                        double ymin, double ymax,
                                     const std::vector<double>& displayValues)
{
  // See pMapPlot::setMacthingRange() for the reason of this shift
  m_eventDisplay -> setRange (xmin + 0.5, xmax + 0.5, ymin + 0.5, ymax + 0.5);
  // A small padding avoid unwanted truncament to the wrong unsigned int 
  m_eventDisplay -> setSize (xmax - xmin + 1.001, ymax - ymin + 1.001);
  m_eventDisplay -> updateData (displayValues);
  m_eventDisplay -> replot();
}


void xpemonPlotGrid::resetPlot()
{
  m_pulseHeightHist -> reset();
  m_pulseHeightPlot -> updateDisplay();
  m_pulseHeightPlot -> replot();
  m_windowSizeHist -> reset();
  m_windowSizePlot -> updateDisplay();
  m_windowSizePlot -> replot();
  m_hitMap -> reset();
  m_hitMapPlot -> updateDisplay();
  m_hitMapPlot -> replot();
  m_eventDisplay -> clearMap();
}

