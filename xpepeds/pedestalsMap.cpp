#include "pedestalsMap.h"
#include <iostream>

using namespace pedestals;

PedestalsMapData::PedestalsMapData(arr_t const &data) : 
      QCPColorMapData(kNx, kNy, QCPRange(1., static_cast<double>(kNx)),
                                QCPRange(1., static_cast<double>(kNy)))
{
  for (int xIndex=0; xIndex<kNx; ++xIndex)
  {
    for (int yIndex=0; yIndex<kNy; ++yIndex)
    {
      int pedestalIndex = kNy * xIndex + yIndex;
      this->setCell(xIndex, yIndex, data.at(pedestalIndex));
    }
  }
}


PedestalsMap::PedestalsMap(QCustomPlot* parentPlot, 
                           PedestalsMapData* data,
                           std::shared_ptr<pColorMapOptions> options) :
                             m_options(options)
{
  // configure axis rect:
  parentPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
  parentPlot->axisRect()->setupFullAxesBox(true);
  parentPlot->xAxis->setLabel(m_options->m_xTitle);
  parentPlot->yAxis->setLabel(m_options->m_yTitle);
   
  // set up the QCPColorMap:
  m_colorMap  = new QCPColorMap(parentPlot->xAxis, parentPlot->yAxis);
  parentPlot->addPlottable(m_colorMap);
  
  m_colorMap->setData(data, false); //No copy of the data involved, we are just taking ownership of it!
  
  // add a color scale:
  m_colorScale  = new QCPColorScale(parentPlot);
  parentPlot->plotLayout()->addElement(0, 1, m_colorScale); // add it to the right of the main axis rect
  m_colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
  m_colorMap->setColorScale(m_colorScale); // associate the color map with the color scale
  m_colorScale->axis()->setLabel(m_options->m_zTitle);
   
  // set the color gradient of the color map to one of the presets:
  m_colorMap->setGradient(m_options->m_gradientType);
   
  // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
  m_colorMap->rescaleDataRange(true);
   
  // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
  m_marginGroup = new QCPMarginGroup(parentPlot);
  parentPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, m_marginGroup);
  m_colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, m_marginGroup);
   
  // rescale the key (x) and value (y) axes so the whole color map is visible:
  parentPlot->rescaleAxes();
}
