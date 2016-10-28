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

pInfoBoxWidget::pInfoBoxWidget(QWidget *parent):
  pInfoBoxGui(parent)
{
  setTitle("Single-event display");

  m_evtCounter = 0;
  
  const QChar MathSymbolmu(0x03BC);
  m_timeLabelName = QString("Time [") + QString(MathSymbolmu) + QString("s]");
  //m_timeLabelName = "Time [s]";
  m_absorptionPointLabelName = "Absorption point [mm]";
  m_pulseHeightLabelName = "Energy [ADC counts]";
  m_phiLabelName = "Polarization angle [deg]";

  //m_counterLabelName = "Event number";
  m_windowSizeLabelName = "Window size";
  m_maxPosLabelName = "Max position [px]";
  m_clusterSizeLabelName = "Cluster size";
  m_baricenterPosLabelName = "Baricenter position [mm]";
  m_mom2TransLabelName = "Second moment (long.)";
  m_mom2LongLabelName = "Second moment (trans.)";
  m_momRatioLabelName = "Elongation";
  m_skewnessLabelName = "Skweness";

  addField(m_timeLabelName);
  addField(m_absorptionPointLabelName);
  addField(m_pulseHeightLabelName);
  addField(m_phiLabelName);

  addSpace();

  //addField(m_counterLabelName);
  addField(m_windowSizeLabelName);
  addField(m_maxPosLabelName);
  addField(m_clusterSizeLabelName);
  addField(m_baricenterPosLabelName);
  addField(m_mom2TransLabelName);  
  addField(m_mom2LongLabelName);
  addField(m_momRatioLabelName);
  addField(m_skewnessLabelName);

  addSpace();
  
  m_drawFirstPassCheckBox = new QCheckBox("Draw first-pass moments analysis");
  addWidget(m_drawFirstPassCheckBox, rowCount(), 0, 1, 2);
  m_drawSearchRegionCheckBox =
    new QCheckBox("Draw absorption-point search region");
  addWidget(m_drawSearchRegionCheckBox, rowCount(), 0, 1, 2);
  m_drawSecondPassCheckBox = new QCheckBox("Draw second-pass moments analysis");
  addWidget(m_drawSecondPassCheckBox, rowCount(), 0, 1, 2);
  initializeText();
}


void pInfoBoxWidget::checkCheckBoxes(bool checked)
{
  m_drawFirstPassCheckBox->setChecked(checked);
  m_drawSearchRegionCheckBox->setChecked(checked);
  m_drawSecondPassCheckBox->setChecked(checked);
}
 

void pInfoBoxWidget::initializeText()
{
  setField(m_timeLabelName, 0);
  setField(m_absorptionPointLabelName, coordinateStringFormat(0., 0.));
  setField(m_pulseHeightLabelName, 0.);
  setField(m_phiLabelName, 0.);
  
  //setField(m_counterLabelName, 0);
  setField(m_windowSizeLabelName, 0);
  setField(m_maxPosLabelName, coordinateStringFormat(0., 0.));
  setField(m_clusterSizeLabelName, 0.);
  setField(m_baricenterPosLabelName, coordinateStringFormat(0., 0.));
  setField(m_mom2TransLabelName, 0.);
  setField(m_mom2LongLabelName, 0.);
  setField(m_momRatioLabelName, 0.);
  setField(m_skewnessLabelName, 0.);
}


void pInfoBoxWidget::updateCounter()
{
  // This is actually not used, at the moment. Need to think about how we
  // get the actual event number from the event.
  m_evtCounter += 1;
  //setField(m_counterLabelName, (m_evtCounter));
}


void pInfoBoxWidget::updateTime(long int microseconds)
{
  // microseconds is of microsecond_t (a.k.a. unint64_t) type
  setField(m_timeLabelName, microseconds);
}


void pInfoBoxWidget::updateAbsorptionPoint(double x, double y)
{
  setField(m_absorptionPointLabelName, coordinateStringFormat(x, y));
}


void pInfoBoxWidget::updatePulseHeight(int pulseHeight)
{
  setField(m_pulseHeightLabelName, pulseHeight);
}


void pInfoBoxWidget::updatePhi(double phi)
{
  setField(m_phiLabelName, number2String(phi));
}


void pInfoBoxWidget::updateWindowSize(unsigned int colMin,
                                      unsigned int colMax,
                                      unsigned int rowMin,
                                      unsigned int rowMax)
{
  unsigned int windowSize = (colMax - colMin + 1) * (rowMax - rowMin + 1);
  setField(m_windowSizeLabelName, windowSize);
}


void pInfoBoxWidget::updateMaxCoordinates(int col, int row)
{
  setField(m_maxPosLabelName, coordinateStringFormat(col, row, 3));
}


void pInfoBoxWidget::updateClusterSize(int size)
{
  setField(m_clusterSizeLabelName, size);
}


void pInfoBoxWidget::updateBaricenterCoordinates(double x, double y)
{
  setField(m_baricenterPosLabelName, coordinateStringFormat(x, y));
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
