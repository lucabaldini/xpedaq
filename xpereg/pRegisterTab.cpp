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

#include "pRegisterTab.h"


pRegisterTab::pRegisterTab() :
  pQtCustomTab("Register poking")
{
  setupWidgets();
  setupConnections();
  freezeSize(xpolgui::kTabGroupBoxWidth);
}


void pRegisterTab::setupWidgets()
{
  m_groupBoxGridLayout->setColumnMinimumWidth(0, 200);
  m_groupBoxGridLayout->setColumnMinimumWidth(1, 125);
  m_groupBoxGridLayout->setColumnMinimumWidth(2, 125);
  
  m_xLabel = new pQtCustomTextLabel(this, "Address x");
  m_xSpinBox = new QSpinBox(this);
  m_xDisplay = new QLineEdit(this);
  m_xDisplay->setEnabled(false);
  m_xDisplay->setText(xpolgui::kNotAvailable);
  m_groupBoxGridLayout->addWidget(m_xLabel, 0, 0);
  m_groupBoxGridLayout->addWidget(m_xSpinBox, 0, 1);
  m_groupBoxGridLayout->addWidget(m_xDisplay, 0, 2);
  
  m_yLabel = new pQtCustomTextLabel(this, "Address y");
  m_ySpinBox = new QSpinBox(this);
  m_yDisplay = new QLineEdit(this);
  m_yDisplay->setEnabled(false);
  m_yDisplay->setText(xpolgui::kNotAvailable);
  m_groupBoxGridLayout->addWidget(m_yLabel, 1, 0);
  m_groupBoxGridLayout->addWidget(m_ySpinBox, 1, 1);
  m_groupBoxGridLayout->addWidget(m_yDisplay, 1, 2);

  m_configLabel = new pQtCustomTextLabel(this, "Configuration");
  m_configSpinBox = new QSpinBox(this);
  m_configDisplay = new QLineEdit(this);
  m_configDisplay->setEnabled(false);
  m_configDisplay->setText(xpolgui::kNotAvailable);
  m_groupBoxGridLayout->addWidget(m_configLabel, 2, 0);
  m_groupBoxGridLayout->addWidget(m_configSpinBox, 2, 1);
  m_groupBoxGridLayout->addWidget(m_configDisplay, 2, 2);
}


void pRegisterTab::setupConnections()
{

}
