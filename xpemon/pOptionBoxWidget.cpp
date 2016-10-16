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

#include "pOptionBoxWidget.h"

pOptionBoxWidget::pOptionBoxWidget(const pMonitorPreferences &preferences,
                                   QWidget *parent):
                                   pQtGroupBoxWidget(parent),
                                   m_preferences(preferences)
{
  setTitle("Monitor configuration");
  //Socket port option init
  m_socketPortLabel = new pQtCustomTextLabel(this, "Socket Port");
  m_socketPortEdit = new pQtCustomLineEdit<unsigned int>(
                                                  m_preferences.m_socketPort);
  m_socketPortEdit->setRangeMax(65535); // maximum value for Udp socket port
  addWidget(m_socketPortLabel, rowCount(), 0);
  addWidget(m_socketPortEdit, rowCount()-1, 1, 1, 2, Qt::AlignHCenter);  
  //Refresh interval option init
  m_refreshIntervalLabel = new pQtCustomTextLabel(this,
                                                  "Refresh interval (ms)");
  m_refreshIntervalEdit = new pQtCustomLineEdit<double>(
                                             m_preferences.m_refreshInterval);
  addWidget(m_refreshIntervalLabel, rowCount(), 0);
  addWidget(m_refreshIntervalEdit, rowCount() - 1, 1, 1, 2, Qt::AlignHCenter);
  m_refreshIntervalEdit->setRangeMin(500.); // would be too fast otherwise
  //Zero suppression option init
  m_zeroSupThrLabel = new pQtCustomTextLabel(this,"Zero suppression");
  m_zeroSupThrEdit = new pQtCustomLineEdit<unsigned int>(
                                    m_preferences.m_zeroSuppressionThreshold);
  m_zeroSupThrEdit->setRangeMin(0);
  addWidget(m_zeroSupThrLabel, rowCount(), 0);
  addWidget(m_zeroSupThrEdit, rowCount() - 1, 1, 1, 2, Qt::AlignHCenter);
  //Add the min-max title (skipping a row to leave some space)
  QLabel* minLabel = new QLabel("Minimum", this);
  QLabel* maxLabel = new QLabel("Maximum", this);
  addWidget(minLabel, rowCount() + 1, 1, Qt::AlignHCenter);
  addWidget(maxLabel, rowCount() - 1, 2, Qt::AlignHCenter);
  //Setting the blank space amount before the min-max title
  m_groupBoxGridLayout->setRowMinimumHeight(rowCount()- 2, 10);
  //Elongation limits option init
  m_elongationLabel = new pQtCustomTextLabel(this, "Elongation");
  m_elongationLimits = new pMinMaxOptionPair<double>(this,
    m_preferences.m_minElongation, m_preferences.m_maxElongation);
  addWidget(m_elongationLabel, rowCount(), 0);
  addWidget(m_elongationLimits, rowCount() - 1, 1, 1, 2);
  //Cluster size limits
  m_clusterSizeLabel = new pQtCustomTextLabel(this, "Cluster size");
  m_clusterSizeLimits = new pMinMaxOptionPair<int>(this,
    m_preferences.m_minClusterSize, m_preferences.m_maxClusterSize);
  addWidget(m_clusterSizeLabel, rowCount(), 0);
  addWidget(m_clusterSizeLimits, rowCount() - 1, 1, 1 , 2);
  m_clusterSizeLimits->setBottom(0);
  //Pulse height limits option init
  m_pulseHeightLabel = new pQtCustomTextLabel(this, "Pulse height");
  m_pulseHeightLimits = new pMinMaxOptionPair<int>(this,
    m_preferences.m_minPulseHeight, m_preferences.m_maxPulseHeight);
  addWidget(m_pulseHeightLabel, rowCount(), 0);
  addWidget(m_pulseHeightLimits, rowCount() - 1, 1, 1 , 2);
  m_pulseHeightLimits->setBottom(0);
  //Window size limits option init
  m_windowSizeLabel = new pQtCustomTextLabel(this, "Window size");
  m_windowSizeLimits = new pMinMaxOptionPair<int>(this,
    m_preferences.m_minWindowSize, m_preferences.m_maxWindowSize);
  addWidget(m_windowSizeLabel, rowCount(), 0);
  addWidget(m_windowSizeLimits, rowCount() - 1, 1, 1 , 2);
  m_pulseHeightLimits->setBottom(0);
}


void pOptionBoxWidget::activateWidgets()
{
  m_socketPortEdit->setDisabled(false);
  m_refreshIntervalEdit->setDisabled(false);
  m_zeroSupThrEdit->setDisabled(false);
  m_elongationLimits->setDisabled(false);    
  m_clusterSizeLimits->setDisabled(false);
  m_pulseHeightLimits->setDisabled(false);
  m_windowSizeLimits->setDisabled(false);
}


void pOptionBoxWidget::disableWidgets()
{
  m_socketPortEdit->setDisabled(true);
  m_refreshIntervalEdit->setDisabled(true);
  m_zeroSupThrEdit->setDisabled(true);
  m_elongationLimits->setDisabled(true);    
  m_clusterSizeLimits->setDisabled(true);
  m_pulseHeightLimits->setDisabled(true);
  m_windowSizeLimits->setDisabled(true);
}


void pOptionBoxWidget::options(pMonitorPreferences* preferences)
{ 
  /* Read the options inserted by the user in the option boxes.*/
  readSocketPort();
  readRefreshInterval();
  readZeroSupThreshold();
  readElongationLimits();
  readClusterSizeLimits();
  readPulseHeightLimits();
  readWindowSizeLimits();
  (*preferences) = m_preferences;
}


void pOptionBoxWidget::readSocketPort()
{
  m_preferences.m_socketPort = m_socketPortEdit->value();
}


void pOptionBoxWidget::readRefreshInterval()
{
  m_preferences.m_refreshInterval = m_refreshIntervalEdit->value();
}


void pOptionBoxWidget::readZeroSupThreshold()
{
  m_preferences.m_zeroSuppressionThreshold = m_zeroSupThrEdit->value();
}


void pOptionBoxWidget::readElongationLimits()
{
  double min, max;
  m_elongationLimits->readOptions(min, max);
  m_preferences.m_minElongation = min;
  m_preferences.m_maxElongation = max;
}


void pOptionBoxWidget::readClusterSizeLimits()
{
  int min, max;
  m_clusterSizeLimits->readOptions(min, max);
  m_preferences.m_minClusterSize = min;
  m_preferences.m_maxClusterSize = max;
}


void pOptionBoxWidget::readPulseHeightLimits()
{
  int min, max;
  m_pulseHeightLimits->readOptions(min, max);
  m_preferences.m_minPulseHeight = min;
  m_preferences.m_maxPulseHeight = max;
}


void pOptionBoxWidget::readWindowSizeLimits()
{
  int min, max;
  m_windowSizeLimits->readOptions(min, max);
  m_preferences.m_minWindowSize = min;
  m_preferences.m_maxWindowSize = max;
}
