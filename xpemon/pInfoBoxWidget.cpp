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
  m_counterLabelName = QString("N. of events");
  m_windowSizeLabelName = "Window size";
  m_maxPosLabelName = "Max position";
  m_clusterSizeLabelName = "Cluster size";
  m_barycenterPosLabelName = "Barycenter position";
  m_pulseHeightLabelName = "Pulse height";
  
  addField(m_counterLabelName);
  addField(m_windowSizeLabelName);
  addField(m_maxPosLabelName);
  addField(m_clusterSizeLabelName);
  addField(m_barycenterPosLabelName);
  addField(m_pulseHeightLabelName);
  
  //m_nEventsLabel = new pQtCustomTextLabel(this, "n. Events");
  //m_nEventsEdit = new QLineEdit();
  //m_nEventsEdit -> setReadOnly(true);
  
  //m_windowSizeLabel = new pQtCustomTextLabel(this, "Window size");
  //m_windowSizeEdit = new QLineEdit();
  //m_windowSizeEdit -> setReadOnly(true);
  
  //m_highestPixelLabel = new pQtCustomTextLabel(this, "Max position");
  //m_highestXEdit = new QLineEdit();
  //m_highestYEdit = new QLineEdit();
  //m_highestXEdit -> setReadOnly(true);
  //m_highestYEdit -> setReadOnly(true);
  
  //m_barycenterLabel = new pQtCustomTextLabel(this, "Barycenter position");
  //m_barycenterXEdit = new QLineEdit();
  //m_barycenterYEdit = new QLineEdit();
  //m_barycenterXEdit -> setReadOnly(true);
  //m_barycenterYEdit -> setReadOnly(true);
  
  //addWidget(m_nEventsLabel, 0, 0);
  //addWidget(m_nEventsEdit, 0, 1);
  //addWidget(m_windowSizeLabel, 1, 0);
  //addWidget(m_windowSizeEdit, 1, 1);
  //addWidget(m_highestPixelLabel, 2, 0);
  //addWidget(m_highestXEdit, 3, 0);
  //addWidget(m_highestYEdit, 3, 1);
  
  // Add some vertical space
  //int row = m_groupBoxGridLayout -> rowCount();
  //int verticalPixelSpace  = 15;
  //m_groupBoxGridLayout -> setRowMinimumHeight(row, verticalPixelSpace);
  
  //addWidget(m_barycenterLabel, 5, 0);
  //addWidget(m_barycenterXEdit, 6, 0);
  //addWidget(m_barycenterYEdit, 6, 1);
  
  initializeText();
}


void pInfoBoxWidget::initializeText()
{
  setField(m_counterLabelName, 0);
  setField(m_windowSizeLabelName, 0);
  setField(m_maxPosLabelName, 0.);
  setField(m_clusterSizeLabelName, 0.);
  setField(m_barycenterPosLabelName, 0.);
  setField(m_pulseHeightLabelName, 0.);
  //m_nEventsEdit -> setText("0");
  //m_windowSizeEdit -> setText("0");
  //m_highestXEdit -> setText("0");
  //m_highestYEdit -> setText("0");
  //m_barycenterXEdit -> setText("0");
  //m_barycenterYEdit -> setText("0");
}


void pInfoBoxWidget::updateCounter()
{
  m_evtCounter += 1;
  //m_nEventsEdit -> setText(QString::number(m_evtCounter));
  setField(m_counterLabelName, (m_evtCounter));
}


void pInfoBoxWidget::updateWindowSize(unsigned int colMin,
                                      unsigned int colMax,
                                      unsigned int rowMin,
                                      unsigned int rowMax)
{
  unsigned int windowSize = (colMax - colMin + 1) * (rowMax - rowMin + 1);
  //m_windowSizeEdit -> setText(QString::number(windowSize));
  setField(m_windowSizeLabelName, windowSize);
}


void pInfoBoxWidget::updateMaxCoordinates(double x, double y)
{
  setField(m_maxPosLabelName, x);
  //m_highestXEdit -> setText(QString::number(x, 'f', 2));
  //m_highestYEdit -> setText(QString::number(y, 'f', 2));
}


void pInfoBoxWidget::updateClusterSize(int size)
{
  setField(m_clusterSizeLabelName, size);
}


void pInfoBoxWidget::updateBarycenterCoordinates(double x, double y)
{
  setField(m_barycenterPosLabelName, x);
  //m_barycenterXEdit -> setText(QString::number(x, 'f', 2));
  //m_barycenterYEdit -> setText(QString::number(y, 'f', 2));
}


void pInfoBoxWidget::updatePulseHeight(int pulseHeight)
{
  setField(m_pulseHeightLabelName, pulseHeight);
}


void pInfoBoxWidget::reset()
{
  m_evtCounter = 0;
  initializeText();
}
