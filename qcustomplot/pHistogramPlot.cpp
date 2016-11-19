/***********************************************************************
Copyright (C) 2007--2016 the X-ray Polarimetry Explorer (XPE) team.

For the license terms see the file LICENSE, distributed along with this
software.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
***********************************************************************/


#include "pHistogramPlot.h"

pHistogramPlot::pHistogramPlot(const pHistogram* hist, 
                               pBasicPlotOptions options) :
                               pCustomHistogramPlot(options),
                               m_hist(hist)
{
  m_bars->keyAxis()->setRange(m_hist->xMin(), m_hist->xMax());
  m_bars->setWidth(m_hist->binWidth());
  setTolerance (TOLERANCE_FACTOR * (m_hist->binWidth()));
}


unsigned int pHistogramPlot::entries() const
{
  return m_hist->entries();
}


double pHistogramPlot::sum() const
{
  return m_hist->sum();
}


void pHistogramPlot::resetView()
{
  m_bars->keyAxis()->setRange(m_hist->xMin(), m_hist->xMax());
  //m_bars->rescaleValueAxis();
  // I am leaving some margin on the top in order to have space for the
  // stat box. This should be configurable.
  yAxis->setRange(0, 1.25*m_hist->maxValue());
  replot();
}


void pHistogramPlot::updateDisplay()
{
  for (unsigned int bin = 0; bin < (m_hist->nbins()); ++bin){
      double key = m_hist->binCenter(bin);
      setKeyContent(key, m_hist->binContent(bin));
  }
  resetView();
}


/*
void pHistogramPlot::setupStatBox()
{
  m_statLabel = new QCPItemText(this);
  addItem(m_statLabel);
  // add the text label at the top:
  m_statLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);
  // place position at center/top of axis rect
  m_statLabel->position->setType(QCPItemPosition::ptAxisRectRatio);   
  m_statLabel->position->setCoords(0.5, 0);
  // make font a bit larger  
  m_statLabel->setFont(QFont(font().family(), 10));
  // show black border around text
  m_statLabel->setPen(QPen(Qt::black)); 
}
*/


void pHistogramPlot::reset()
{
  //m_hist->reset();
  clearBars();
}

