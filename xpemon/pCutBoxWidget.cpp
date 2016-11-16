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

#include "pCutBoxWidget.h"


pCutBoxWidget::pCutBoxWidget(const pMonitorPreferences &preferences,
			     QWidget *parent) :
  pQtGroupBoxWidget(parent),
  m_preferences(preferences)
{
  setTitle("Quality cuts");
  //Add the min-max title (skipping a row to leave some space)
  QLabel* minLabel = new QLabel("Minimum", this);
  QLabel* maxLabel = new QLabel("Maximum", this);
  addWidget(minLabel, 0, 1);
  addWidget(maxLabel, 0, 2);
  //Elongation limits option init
  m_elongationLabel = new pQtCustomTextLabel(this, "Elongation");
  m_elongationLimits = new pMinMaxOptionPair<double>(this,
    m_preferences.m_minElongation, m_preferences.m_maxElongation);
  addWidget(m_elongationLabel, 1, 0);
  addWidget(m_elongationLimits->minEdit(), 1, 1);
  addWidget(m_elongationLimits->maxEdit(), 1, 2);
  //Cluster size limits
  m_clusterSizeLabel = new pQtCustomTextLabel(this, "Cluster size");
  m_clusterSizeLimits = new pMinMaxOptionPair<int>(this,
    m_preferences.m_minClusterSize, m_preferences.m_maxClusterSize);
  addWidget(m_clusterSizeLabel, 2, 0);
  addWidget(m_clusterSizeLimits->minEdit(), 2, 1);
  addWidget(m_clusterSizeLimits->maxEdit(), 2, 2);
  m_clusterSizeLimits->setBottom(0);
  //Pulse height limits option init
  m_pulseHeightLabel = new pQtCustomTextLabel(this, "Pulse height");
  m_pulseHeightLimits = new pMinMaxOptionPair<int>(this,
    m_preferences.m_minPulseHeight, m_preferences.m_maxPulseHeight);
  addWidget(m_pulseHeightLabel, 3, 0);
  addWidget(m_pulseHeightLimits->minEdit(), 3, 1);
  addWidget(m_pulseHeightLimits->maxEdit(), 3, 2);
  m_pulseHeightLimits->setBottom(0);
  //Window size limits option init
  m_windowSizeLabel = new pQtCustomTextLabel(this, "Window size");
  m_windowSizeLimits = new pMinMaxOptionPair<int>(this,
    m_preferences.m_minWindowSize, m_preferences.m_maxWindowSize);
  addWidget(m_windowSizeLabel, 4, 0);
  addWidget(m_windowSizeLimits->minEdit(), 4, 1);
  addWidget(m_windowSizeLimits->maxEdit(), 4, 2);
  m_windowSizeLimits->setBottom(0);
  //Skewness limits option init
  m_skewnessLabel = new pQtCustomTextLabel(this, "Skewness (abs)");
  m_skewnessLimits = new pMinMaxOptionPair<double>(this,
    m_preferences.m_minSkewness, m_preferences.m_maxSkewness);
  addWidget(m_skewnessLabel, 5, 0);
  addWidget(m_skewnessLimits->minEdit(), 5, 1);
  addWidget(m_skewnessLimits->maxEdit(), 5, 2);
  m_skewnessLimits->setBottom(0);

  freezeHeight();
}


void pCutBoxWidget::enable()
{
  m_elongationLimits->setEnabled(true);
  m_clusterSizeLimits->setEnabled(true);
  m_pulseHeightLimits->setEnabled(true);
  m_windowSizeLimits->setEnabled(true);
  m_skewnessLimits->setEnabled(true);
}


void pCutBoxWidget::disable()
{
  m_elongationLimits->setDisabled(true);    
  m_clusterSizeLimits->setDisabled(true);
  m_pulseHeightLimits->setDisabled(true);
  m_windowSizeLimits->setDisabled(true);
  m_skewnessLimits->setDisabled(true);
}


void pCutBoxWidget::options(pMonitorPreferences* preferences)
{ 
  /* Read the options inserted by the user in the option boxes.*/
  readElongationLimits();
  readClusterSizeLimits();
  readPulseHeightLimits();
  readWindowSizeLimits();
  readSkewnessLimits();
  preferences->m_minElongation = m_preferences.m_minElongation;
  preferences->m_maxElongation = m_preferences.m_maxElongation;
  preferences->m_minClusterSize = m_preferences.m_minClusterSize;
  preferences->m_maxClusterSize = m_preferences.m_maxClusterSize;
  preferences->m_minPulseHeight = m_preferences.m_minPulseHeight;
  preferences->m_maxPulseHeight = m_preferences.m_maxPulseHeight;
  preferences->m_minWindowSize = m_preferences.m_minWindowSize;
  preferences->m_maxWindowSize = m_preferences.m_maxWindowSize;
  preferences->m_minSkewness = m_preferences.m_minSkewness;
  preferences->m_maxSkewness = m_preferences.m_maxSkewness;
}


void pCutBoxWidget::readElongationLimits()
{
  double min, max;
  m_elongationLimits->readOptions(min, max);
  m_preferences.m_minElongation = min;
  m_preferences.m_maxElongation = max;
}


void pCutBoxWidget::readClusterSizeLimits()
{
  int min, max;
  m_clusterSizeLimits->readOptions(min, max);
  m_preferences.m_minClusterSize = min;
  m_preferences.m_maxClusterSize = max;
}


void pCutBoxWidget::readPulseHeightLimits()
{
  int min, max;
  m_pulseHeightLimits->readOptions(min, max);
  m_preferences.m_minPulseHeight = min;
  m_preferences.m_maxPulseHeight = max;
}


void pCutBoxWidget::readWindowSizeLimits()
{
  int min, max;
  m_windowSizeLimits->readOptions(min, max);
  m_preferences.m_minWindowSize = min;
  m_preferences.m_maxWindowSize = max;
}


void pCutBoxWidget::readSkewnessLimits()
{
  double min, max;
  m_skewnessLimits->readOptions(min, max);
  m_preferences.m_minSkewness = min;
  m_preferences.m_maxSkewness = max;
}
