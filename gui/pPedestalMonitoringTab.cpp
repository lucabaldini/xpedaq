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

#include "pPedestalMonitoringTab.h"

pPedestalMonitoringTab::pPedestalMonitoringTab() :
  pQtCustomTab("Pedestal Monitoring")
{
  setupOutlierThresholdButton();
  setupConnections();
}


/*!
 */
void pPedestalMonitoringTab::setupOutlierThresholdButton()
{
  int row = m_groupBoxGridLayout->rowCount();
  m_outlierThresholdSpinBox = new QSpinBox(this);
  m_outlierThresholdLabel = new pQtCustomTextLabel(this, "Outlier threshold");
  m_groupBoxGridLayout->addWidget(m_outlierThresholdLabel, row, 0, 1, 2);
  m_groupBoxGridLayout->addWidget(m_outlierThresholdSpinBox, row, 3, 1, 1);
  m_outlierThresholdSpinBox->setMinimum(0);
  m_outlierThresholdSpinBox->setMaximum(100);
  m_outlierThresholdSpinBox->setValue(10);
}


/*!
 */
void pPedestalMonitoringTab::setupConnections()
{
}


/*!
 */
void pPedestalMonitoringTab::enableAll()
{
  m_outlierThresholdSpinBox->setEnabled(true);
}


/*!
 */
void pPedestalMonitoringTab::disableAll()
{
  m_outlierThresholdSpinBox->setEnabled(false);
}


/*!
 */
void pPedestalMonitoringTab::displayConfiguration()
{
  
}


/*!
 */
int pPedestalMonitoringTab::getThreshold()
{
  return m_outlierThresholdSpinBox->value();
}
