#include "xpemonPlotGrid.h"

xpemonPlotGrid::xpemonPlotGrid(QWidget *parent) : QWidget(parent)
{
  m_PlotLayout = new QGridLayout(this);
  m_pulseHeightPlot = new PulseHeightPlot();
  m_PlotLayout->addWidget(m_pulseHeightPlot, 0, 0);
  
  pColorMapOptions* barycenterOpt = new pColorMapOptions();
  m_barycenterPlot = new BarycenterPlot(barycenterOpt);
  m_PlotLayout->addWidget(m_barycenterPlot, 0, 1, 1, 1);
  m_pulseHeightPlot->resize(400, 200);
  m_barycenterPlot->resize(400, 200);
}


void xpemonPlotGrid::addPulseHeightPoint(int pHeight)
{
  m_pulseHeightPlot -> addPoint(pHeight);
}


void xpemonPlotGrid::addBarycenterPoint(double xBar, double yBar)
{
  m_barycenterPlot -> addPoint(xBar,yBar);
}


void xpemonPlotGrid::refreshPlot()
{
  std::cout<<"Updating plots" << std::endl;
  m_pulseHeightPlot->replot();
  m_barycenterPlot->replot(); 
}

void xpemonPlotGrid::resetPlot()
{
  m_pulseHeightPlot->reset();
  m_barycenterPlot->reset(); 
}

