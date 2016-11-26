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

#include "xperegRegisterTab.h"


xperegRegisterTab::xperegRegisterTab() :
  pQtCustomTab("Register poking")
{
  setupWidgets();
  setupConnections();
  freezeSize(xpolgui::kTabGroupBoxWidth);
}


void xperegRegisterTab::setupWidgets()
{
  m_groupBoxGridLayout->setColumnMinimumWidth(0, 200);
  m_groupBoxGridLayout->setColumnMinimumWidth(1, 125);
  m_groupBoxGridLayout->setColumnMinimumWidth(2, 125);

  int row = rowCount();
  m_xLabel = new pQtCustomTextLabel(this, "Address x");
  m_xSpinBox = new QSpinBox(this);
  m_xSpinBox->setMinimum(0);
  m_xSpinBox->setMaximum(xpoldetector::kNumPixelsX - 1);
  m_xDisplay = new QLineEdit(this);
  m_xDisplay->setEnabled(false);
  m_xDisplay->setText(xpolgui::kNotAvailable);
  m_groupBoxGridLayout->addWidget(m_xLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_xSpinBox, row, 1);
  m_groupBoxGridLayout->addWidget(m_xDisplay, row, 2);

  row = rowCount();
  m_yLabel = new pQtCustomTextLabel(this, "Address y");
  m_ySpinBox = new QSpinBox(this);
  m_ySpinBox->setMinimum(0);
  m_ySpinBox->setMaximum(xpoldetector::kNumPixelsY - 1);
  m_yDisplay = new QLineEdit(this);
  m_yDisplay->setEnabled(false);
  m_yDisplay->setText(xpolgui::kNotAvailable);
  m_groupBoxGridLayout->addWidget(m_yLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_ySpinBox, row, 1);
  m_groupBoxGridLayout->addWidget(m_yDisplay, row, 2);

  row = rowCount();
  m_configLabel = new pQtCustomTextLabel(this, "Configuration");
  m_configSpinBox = new QSpinBox(this);
  m_configSpinBox->setMinimum(0);
  m_configSpinBox->setMaximum(65535);
  m_configDisplay = new QLineEdit(this);
  m_configDisplay->setEnabled(false);
  m_configDisplay->setText(xpolgui::kNotAvailable);
  m_groupBoxGridLayout->addWidget(m_configLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_configSpinBox, row, 1);
  m_groupBoxGridLayout->addWidget(m_configDisplay, row, 2);

  addVerticalSpacer();

  row = rowCount();
  m_randomCheckBox = new QCheckBox("Change values randomly");
  m_groupBoxGridLayout->addWidget(m_randomCheckBox, row, 0);

  addVerticalSpacer();

  row = rowCount();
  m_numReadoutsLabel = new pQtCustomTextLabel(this, "Readouts per poke");
  m_numReadoutsSpinBox = new QSpinBox(this);
  m_numReadoutsSpinBox->setMinimum(0);
  m_numReadoutsSpinBox->setMaximum(999999);
  m_numReadoutsSpinBox->setValue(100);
  m_groupBoxGridLayout->addWidget(m_numReadoutsLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_numReadoutsSpinBox, row, 1);

  row = rowCount();
  m_readoutIntervalLabel = new pQtCustomTextLabel(this, "Readout interval");
  m_readoutIntervalSpinBox = new QSpinBox(this);
  m_readoutIntervalSpinBox->setMinimum(10);
  m_readoutIntervalSpinBox->setMaximum(10000);
  m_readoutIntervalSpinBox->setValue(100);
  m_readoutIntervalSpinBox->setSuffix(" ms");
  m_groupBoxGridLayout->addWidget(m_readoutIntervalLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_readoutIntervalSpinBox, row, 1);
}


void xperegRegisterTab::setupConnections()
{

}



void xperegRegisterTab::displayUserPreferences(xperegUserPreferences
					       *preferences)
{
  m_xSpinBox->setValue(preferences->m_pixelAddressX);
  m_ySpinBox->setValue(preferences->m_pixelAddressY);
  m_configSpinBox->setValue(preferences->m_configuration);
  m_randomCheckBox->setChecked(preferences->m_randomShuffle);
  m_numReadoutsSpinBox->setValue(preferences->m_numReadouts);
  m_readoutIntervalSpinBox->setValue(preferences->m_readoutInterval);
}
