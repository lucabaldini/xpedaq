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

#include "pInfoBoxWidget.h"

pInfoBoxWidget::pInfoBoxWidget(QWidget *parent): pInfoBoxGui(parent)
{
  m_evtCounter = 0;
  m_counterLabelName = "N. of events";
  m_windowSizeLabelName = "Window size";
  m_maxPosLabelName = "Max position";
  m_clusterSizeLabelName = "Cluster size";
  m_barycenterPosLabelName = "Barycenter position";
  m_pulseHeightLabelName = "Pulse height";
  m_angleLabelName = "Phi";
  m_mom2TransLabelName = "Mom. trans.";
  m_mom2LongLabelName = "Mom. long.";
  m_momRatioLabelName = "Mom. ratio";
  m_skewnessLabelName = "Skweness";
  
  addField(m_counterLabelName);
  addField(m_windowSizeLabelName);
  addField(m_maxPosLabelName);
  addField(m_clusterSizeLabelName);
  addField(m_barycenterPosLabelName);
  addField(m_pulseHeightLabelName);
  addField(m_angleLabelName);
  addField(m_mom2TransLabelName);  
  addField(m_mom2LongLabelName);
  addField(m_momRatioLabelName);
  addField(m_skewnessLabelName);   
  initializeText();
}


void pInfoBoxWidget::initializeText()
{
  setField(m_counterLabelName, 0);
  setField(m_windowSizeLabelName, 0);
  setField(m_maxPosLabelName, coordinateStringFormat(0., 0.));
  setField(m_clusterSizeLabelName, 0.);
  setField(m_barycenterPosLabelName, coordinateStringFormat(0., 0.));
  setField(m_pulseHeightLabelName, 0);
  setField(m_angleLabelName, 0.);
  setField(m_mom2TransLabelName, 0.);
  setField(m_mom2LongLabelName, 0.);
  setField(m_momRatioLabelName, 0.);
  setField(m_skewnessLabelName, 0.);
}


void pInfoBoxWidget::updateCounter()
{
  m_evtCounter += 1;
  setField(m_counterLabelName, (m_evtCounter));
}


void pInfoBoxWidget::updateWindowSize(unsigned int colMin,
                                      unsigned int colMax,
                                      unsigned int rowMin,
                                      unsigned int rowMax)
{
  unsigned int windowSize = (colMax - colMin + 1) * (rowMax - rowMin + 1);
  setField(m_windowSizeLabelName, windowSize);
}


void pInfoBoxWidget::updateMaxCoordinates(double x, double y)
{
  setField(m_maxPosLabelName, coordinateStringFormat(x, y));
}


void pInfoBoxWidget::updateClusterSize(int size)
{
  setField(m_clusterSizeLabelName, size);
}


void pInfoBoxWidget::updateBarycenterCoordinates(double x, double y)
{
  setField(m_barycenterPosLabelName, coordinateStringFormat(x, y));
}


void pInfoBoxWidget::updatePulseHeight(int pulseHeight)
{
  setField(m_pulseHeightLabelName, pulseHeight);
}


void pInfoBoxWidget::updatePhi(double phi)
{
  setField(m_angleLabelName, number2String(phi));
}


void pInfoBoxWidget::updateMom2Trans(double mom2Trans)
{
  setField(m_mom2TransLabelName, number2String(mom2Trans));
}


void pInfoBoxWidget::updateMom2Long(double mom2Long)
{
  setField(m_mom2LongLabelName, number2String(mom2Long));
}


void pInfoBoxWidget::updateMomRatio(double momRatio)
{
  setField(m_momRatioLabelName, number2String(momRatio));
}


void pInfoBoxWidget::updateSkewness(double skewness)
{
  setField(m_skewnessLabelName, number2String(skewness));
}


void pInfoBoxWidget::reset()
{
  m_evtCounter = 0;
  initializeText();
}
