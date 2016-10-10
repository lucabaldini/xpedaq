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

//pOptionBoxWidget::pOptionBoxWidget(unsigned int socketPort,
//                                   double refreshInterval,
//                                   unsigned int  zeroSupThreshold,
//                                   QWidget *parent):
//                                   pQtGroupBoxWidget(parent)
pOptionBoxWidget::pOptionBoxWidget(const pMonitorPreferences &preferences,
                                   QWidget *parent):
                                   pQtGroupBoxWidget(parent)
{
  m_socketPortLabel = new pQtCustomTextLabel(this, "Socket Port");
  m_socketPortEdit = new QLineEdit();
  m_refreshIntervalLabel = new pQtCustomTextLabel(this,
                                             "Refresh interval (ms)");
  m_refreshIntervalEdit = new QLineEdit();
  m_zeroSupThrLabel = new pQtCustomTextLabel(this,"Zero suppression");
  m_zeroSupThrEdit = new QLineEdit();
  m_drawReconInfoCheckBox = new QCheckBox("Draw recon");
  addWidget(m_socketPortLabel, 0,0);
  addWidget(m_socketPortEdit, 0,1);
  addWidget(m_refreshIntervalLabel, 1,0);
  addWidget(m_refreshIntervalEdit, 1,1);
  addWidget(m_zeroSupThrLabel, 2,0);
  addWidget(m_zeroSupThrEdit, 2,1);
  addWidget(m_drawReconInfoCheckBox, 3, 0);
  
  // Display the initial values
  m_socketPortText.setNum(preferences.socketPort());
  m_refreshIntervalText.setNum(preferences.refreshInterval());
  m_zeroSupThresholdText.setNum(preferences.zeroSuppressionThreshold());
  //m_socketPortText.setNum(preferences.socketPort());
  //m_refreshIntervalText.setNum(refreshInterval());
  //m_zeroSupThresholdText.setNum(zeroSupThreshold());
  initalizeText();
  connect (m_drawReconInfoCheckBox, SIGNAL(stateChanged(int)),
           this, SLOT(updateReconInfoBoxStatus(int)));
}


void pOptionBoxWidget::initalizeText()
{
  m_socketPortEdit->setText(m_socketPortText);
  m_refreshIntervalEdit->setText(m_refreshIntervalText);
  m_zeroSupThrEdit->setText(m_zeroSupThresholdText);
}


void pOptionBoxWidget::activateWidgets()
{
  m_socketPortEdit->setDisabled(false);
  m_refreshIntervalEdit->setDisabled(false);
  m_zeroSupThrEdit->setDisabled(false);  
}


void pOptionBoxWidget::disableWidgets()
{
  m_socketPortEdit->setDisabled(true);
  m_refreshIntervalEdit->setDisabled(true);
  m_zeroSupThrEdit->setDisabled(true);  
}


void pOptionBoxWidget::options(pMonitorPreferences* preferences)
{ 
  /* Read the options inserted by the user in the option boxes.
     If an option is invalid restore the last valid value inserted */
  unsigned int socketPort;
  double refreshInterval;
  unsigned int zeroSupThreshold;  
  readSocketPort(socketPort);
  readRefreshInterval(refreshInterval);
  readZeroSupThreshold(zeroSupThreshold);
  preferences->setSocketPort(socketPort);
  preferences->setRefreshInterval(refreshInterval);
  preferences->setZeroSuppressionThreshold(zeroSupThreshold);
}


//void pOptionBoxWidget::options(unsigned int &socketPort,
//                               double &refreshInterval,
//                               unsigned int  &zeroSupThreshold)
//{ 
//  /* Read the options inserted by the user in the option boxes.
//     If an option is invalid restore the last valid value inserted */
//  readSocketPort(socketPort);
//  readRefreshInterval(refreshInterval);
//  readZeroSupThreshold(zeroSupThreshold);
//}


void pOptionBoxWidget::readSocketPort(unsigned int &socketPort)
{
  bool convSuccess;
  socketPort = (m_socketPortEdit->text()).toUInt(&convSuccess);
  if (!convSuccess)
  {
    socketPort = m_socketPortText.toUInt();
    m_socketPortEdit->setText(m_socketPortText);
    return;
  }
  if (socketPort > 65535)  // maximum value for Udp socket port
  {
    socketPort = m_socketPortText.toUInt();
    m_socketPortEdit->setText(m_socketPortText);
    return;
  }
  m_socketPortText = m_socketPortEdit->text();
}


void pOptionBoxWidget::readRefreshInterval(double &refreshInterval)
{
  bool convSuccess;
  refreshInterval = (m_refreshIntervalEdit->text()).toDouble(&convSuccess);
  if (!convSuccess)
  {
    refreshInterval = m_refreshIntervalText.toDouble();
    m_refreshIntervalEdit->setText(m_refreshIntervalText);
    return;
  }
  if (refreshInterval <= 0.)
  {
    refreshInterval = m_refreshIntervalText.toDouble();
    m_refreshIntervalEdit->setText(m_refreshIntervalText);
    return;
  }
  m_refreshIntervalText = m_refreshIntervalEdit->text();
}


void pOptionBoxWidget::readZeroSupThreshold(unsigned int &zeroSupThreshold)
{
  bool convSuccess;
  zeroSupThreshold = (m_zeroSupThrEdit->text()).toUInt(&convSuccess);
  if (!convSuccess)
  {
    zeroSupThreshold = m_zeroSupThresholdText.toUInt();;
    m_zeroSupThrEdit->setText(m_zeroSupThresholdText);
  }
  m_zeroSupThresholdText = m_zeroSupThrEdit->text();
}


void pOptionBoxWidget::updateReconInfoBoxStatus(int state)
{
  emit drawReconInfoCheckBoxStatusChanged(state);
}
