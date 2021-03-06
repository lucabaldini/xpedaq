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
  m_groupBoxGridLayout->setColumnMinimumWidth(0, 250);
  int row = rowCount();
  m_xLabel = new pQtCustomTextLabel(this, "Address x");
  m_xSpinBox = new QSpinBox(this);
  m_xSpinBox->setMinimum(0);
  m_xSpinBox->setMaximum(xpoldetector::kNumPixelsX - 1);
  m_groupBoxGridLayout->addWidget(m_xLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_xSpinBox, row, 1);
  row = rowCount();
  m_yLabel = new pQtCustomTextLabel(this, "Address y");
  m_ySpinBox = new QSpinBox(this);
  m_ySpinBox->setMinimum(0);
  m_ySpinBox->setMaximum(xpoldetector::kNumPixelsY - 1);
  m_groupBoxGridLayout->addWidget(m_yLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_ySpinBox, row, 1);
  row = rowCount();
  m_configLabel = new pQtCustomTextLabel(this, "Configuration");
  m_configSpinBox = new QSpinBox(this);
  m_configSpinBox->setMinimum(0);
  m_configSpinBox->setMaximum(65535);
  m_groupBoxGridLayout->addWidget(m_configLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_configSpinBox, row, 1);

  addVerticalSpacer();

  row = rowCount();
  m_randomCheckBox = new QCheckBox("Change values randomly");
  m_groupBoxGridLayout->addWidget(m_randomCheckBox, row, 0);

  addVerticalSpacer();

  row = rowCount();
  m_readoutRepeatLabel = new pQtCustomTextLabel(this, "Readouts per poke");
  m_readoutRepeatSpinBox = new QSpinBox(this);
  m_readoutRepeatSpinBox->setMinimum(0);
  m_readoutRepeatSpinBox->setMaximum(999999);
  m_readoutRepeatSpinBox->setValue(100);
  m_groupBoxGridLayout->addWidget(m_readoutRepeatLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_readoutRepeatSpinBox, row, 1);
  row = rowCount();
  m_readoutIntervalLabel = new pQtCustomTextLabel(this, "Readout interval");
  m_readoutIntervalSpinBox = new QSpinBox(this);
  m_readoutIntervalSpinBox->setMinimum(200);
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
  m_readoutRepeatSpinBox->setValue(preferences->m_readoutRepeat);
  m_readoutIntervalSpinBox->setValue(preferences->m_readoutInterval);
}


void xperegRegisterTab::updateRegisters(unsigned short x, unsigned short y,
					unsigned short config)
{
  m_xSpinBox->setValue(x);
  m_ySpinBox->setValue(y);
  m_configSpinBox->setValue(config);
}


void xperegRegisterTab::userPreferences(xperegUserPreferences &preferences)
{
  preferences.m_pixelAddressX = pixelAddressX();
  preferences.m_pixelAddressY = pixelAddressY();
  preferences.m_configuration = configuration();
  preferences.m_randomShuffle = randomShuffle();
  preferences.m_readoutRepeat = readoutRepeat();
  preferences.m_readoutInterval = readoutInterval();
}


unsigned short xperegRegisterTab::pixelAddressX() const
{
  return m_xSpinBox->value();
}


unsigned short xperegRegisterTab::pixelAddressY() const
{
  return m_ySpinBox->value();
}


unsigned short xperegRegisterTab::configuration() const
{
  return m_configSpinBox->value();
}


bool xperegRegisterTab::randomShuffle() const
{
  return m_randomCheckBox->isChecked();
}


int xperegRegisterTab::readoutRepeat() const
{
  return m_readoutRepeatSpinBox->value();
}


int xperegRegisterTab::readoutInterval() const
{
  return m_readoutIntervalSpinBox->value();
}
