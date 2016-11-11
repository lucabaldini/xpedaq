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
                                   QWidget *parent) :
  pQtGroupBoxWidget(parent),
  m_preferences(preferences)
{
  setTitle("Monitor configuration");
  //Socket port option init
  m_socketPortLabel = new pQtCustomTextLabel(this, "UDP socket Port");
  m_socketPortEdit = new pQtCustomLineEdit<unsigned int>(
                                                  m_preferences.m_socketPort);
  m_socketPortEdit->setRangeMax(65535); // maximum value for Udp socket port
  addWidget(m_socketPortLabel, 0, 0);
  addWidget(m_socketPortEdit, 0, 1);  
  //Refresh interval option init
  m_refreshIntervalLabel = new pQtCustomTextLabel(this,
                                                  "Refresh interval [ms]");
  m_refreshIntervalEdit = new pQtCustomLineEdit<double>(
                                             m_preferences.m_refreshInterval);
  addWidget(m_refreshIntervalLabel, 1, 0);
  addWidget(m_refreshIntervalEdit, 1, 1);
  m_refreshIntervalEdit->setRangeMin(500.); // would be too fast otherwise
  //Zero suppression option init
  m_zeroSupThrLabel = new pQtCustomTextLabel(this,"Zero suppression");
  m_zeroSupThrEdit = new pQtCustomLineEdit<unsigned int>(
                                    m_preferences.m_zeroSuppressionThreshold);
  m_zeroSupThrEdit->setRangeMin(0);
  addWidget(m_zeroSupThrLabel, 2, 0);
  addWidget(m_zeroSupThrEdit, 2, 1);
  freezeHeight();
}


void pOptionBoxWidget::enable()
{
  m_socketPortEdit->setEnabled(true);
  m_refreshIntervalEdit->setEnabled(true);
  m_zeroSupThrEdit->setEnabled(true);
}


void pOptionBoxWidget::disable()
{
  m_socketPortEdit->setDisabled(true);
  m_refreshIntervalEdit->setDisabled(true);
  m_zeroSupThrEdit->setDisabled(true);
}


void pOptionBoxWidget::options(pMonitorPreferences* preferences)
{ 
  /* Read the options inserted by the user in the option boxes.*/
  readSocketPort();
  readRefreshInterval();
  readZeroSupThreshold();
  preferences->m_socketPort = m_preferences.m_socketPort;
  preferences->m_refreshInterval = m_preferences.m_refreshInterval;
  preferences->m_zeroSuppressionThreshold =
                                    m_preferences.m_zeroSuppressionThreshold;
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
