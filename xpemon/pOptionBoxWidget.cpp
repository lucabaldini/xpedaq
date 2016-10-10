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
                                   pQtGroupBoxWidget(parent),
                                   m_preferences(preferences)
{
  m_socketPortLabel = new pQtCustomTextLabel(this, "Socket Port");
  m_socketPortEdit = new QLineEdit();
  m_refreshIntervalLabel = new pQtCustomTextLabel(this,
                                             "Refresh interval (ms)");
  m_refreshIntervalEdit = new QLineEdit();
  m_zeroSupThrLabel = new pQtCustomTextLabel(this,"Zero suppression");
  m_zeroSupThrEdit = new QLineEdit();
  m_minElongationLabel = new pQtCustomTextLabel(this, "Elongation");
  m_minElongationEdit =  new QLineEdit();
  m_maxElongationEdit =  new QLineEdit();
  m_drawReconInfoCheckBox = new QCheckBox("Draw recon");
  
  addWidget(m_socketPortLabel, 0,0);
  addWidget(m_socketPortEdit, 0,1);
  addWidget(m_refreshIntervalLabel, 1,0);
  addWidget(m_refreshIntervalEdit, 1,1);
  addWidget(m_zeroSupThrLabel, 2,0);
  addWidget(m_zeroSupThrEdit, 2,1);
  addWidget(m_minElongationLabel, 3,0);
  addWidget(m_minElongationEdit, 3,1);
  addWidget(m_maxElongationEdit, 3,2);
  addWidget(m_drawReconInfoCheckBox, 4, 0);
  
  // Display the initial values
  initalizeText();
  
  connect (m_drawReconInfoCheckBox, SIGNAL(stateChanged(int)),
           this, SLOT(updateReconInfoBoxStatus(int)));
}


void pOptionBoxWidget::initalizeText()
{
  m_socketPortEdit->setText(QString::number(m_preferences.socketPort()));  
  m_refreshIntervalEdit->setText(QString::number(
                                            m_preferences.refreshInterval()));
  m_zeroSupThrEdit->setText(QString::number(
                                  m_preferences.zeroSuppressionThreshold()));
  m_minElongationEdit->setText(QString::number(
                                             m_preferences.minElongation()));
  m_maxElongationEdit->setText(QString::number(
                                             m_preferences.maxElongation()));
}


void pOptionBoxWidget::activateWidgets()
{
  m_socketPortEdit->setDisabled(false);
  m_refreshIntervalEdit->setDisabled(false);
  m_zeroSupThrEdit->setDisabled(false);
  m_minElongationEdit->setDisabled(false);
  m_maxElongationEdit->setDisabled(false);
}


void pOptionBoxWidget::disableWidgets()
{
  m_socketPortEdit->setDisabled(true);
  m_refreshIntervalEdit->setDisabled(true);
  m_zeroSupThrEdit->setDisabled(true);
  m_minElongationEdit->setDisabled(true);
  m_maxElongationEdit->setDisabled(true);  
}


void pOptionBoxWidget::options(pMonitorPreferences* preferences)
{ 
  /* Read the options inserted by the user in the option boxes.
     If an option is invalid restore the last valid value inserted */
  readSocketPort();
  readRefreshInterval();
  readZeroSupThreshold();
  readMinElongation();
  readMaxElongation();
  (*preferences) = m_preferences;
}


void pOptionBoxWidget::readSocketPort()
{
  bool convSuccess;
  unsigned int socketPort = (m_socketPortEdit->text()).toUInt(&convSuccess);
  if (!convSuccess || socketPort > 65535) // maximum value for Udp socket port
  {
    m_socketPortEdit->setText(QString::number(m_preferences.socketPort()));
    return;
  }
  m_preferences.setSocketPort(socketPort);
}


void pOptionBoxWidget::readRefreshInterval()
{
  bool convSuccess;
  double refreshInterval = (m_refreshIntervalEdit->text()).toDouble(
                                                                &convSuccess);
  if (!convSuccess || refreshInterval <= 0.)
  {
    m_refreshIntervalEdit->setText(QString::number(
                                           m_preferences.refreshInterval()));
    return;
  }
  m_preferences.setRefreshInterval(refreshInterval);  
}


void pOptionBoxWidget::readZeroSupThreshold()
{
  bool convSuccess;
  unsigned int zeroSupThreshold = (m_zeroSupThrEdit->text()).toUInt(
                                                                &convSuccess);
  if (!convSuccess)
  {
    m_zeroSupThrEdit->setText(QString::number(
                                   m_preferences.zeroSuppressionThreshold()));
  }
  m_preferences.setZeroSuppressionThreshold(zeroSupThreshold);
}


void pOptionBoxWidget::readMinElongation()
{
  bool convSuccess;
  double minElongation = (m_minElongationEdit->text()).toDouble(&convSuccess);
  if (!convSuccess || minElongation <= 0. ||
      minElongation > m_preferences.maxElongation())
  {
    m_minElongationEdit->setText(QString::number(
                                             m_preferences.minElongation()));
    return;
  }
  m_preferences.setMinElongation(minElongation);  
}


void pOptionBoxWidget::readMaxElongation()
{
  bool convSuccess;
  double maxElongation = (m_maxElongationEdit->text()).toDouble(&convSuccess);
  if (!convSuccess || maxElongation <= m_preferences.minElongation())
  {
    m_maxElongationEdit->setText(QString::number(
                                             m_preferences.maxElongation()));
    return;
  }
  m_preferences.setMaxElongation(maxElongation);  
}


void pOptionBoxWidget::updateReconInfoBoxStatus(int state)
{
  emit drawReconInfoCheckBoxStatusChanged(state);
}
