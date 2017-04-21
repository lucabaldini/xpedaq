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

#include "pUsbControlTab.h"


/*!
 */
pUsbControlTab::pUsbControlTab(bool timeoutWidgets)
  : pQtCustomTab("USB management")
{
  m_groupBoxGridLayout->setColumnMinimumWidth(0, 250);
  m_groupBoxGridLayout->setColumnMinimumWidth(2, 30);
  setupWidgets(timeoutWidgets);
  freezeSize(xpolgui::kTabGroupBoxWidth);
}


/*!
 */
int pUsbControlTab::timeout() const
{
  return m_timeoutSpinBox->text().toInt();
}


/*!
 */
void pUsbControlTab::setTimeout(int timeout)
{
  m_timeoutSpinBox->setValue(timeout);
}


/*!
 */
void pUsbControlTab::setupWidgets(bool timeoutWidgets)
{
  int row = m_groupBoxGridLayout->rowCount();
  m_statusLabel = new pQtCustomTextLabel(this, "Connection status");
  m_statusLabel->setLabelStyle();
  m_statusDisplay = new QLineEdit(this);
  m_statusDisplay->setEnabled(false);
  m_statusDisplay->setText("Disconnected");
  m_groupBoxGridLayout->addWidget(m_statusLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_statusDisplay, row, 1, 1, 2);
  row ++;
  m_deviceNameLabel = new pQtCustomTextLabel(this, "USB device name");
  m_deviceNameLabel->setLabelStyle();
  m_deviceNameDisplay = new QLineEdit(this);
  m_deviceNameDisplay->setEnabled(false);
  m_deviceNameDisplay->setText(xpolgui::kNotAvailable);
  m_groupBoxGridLayout->addWidget(m_deviceNameLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_deviceNameDisplay, row, 1, 1, 2);
  row ++;
  m_driverVersionLabel = new pQtCustomTextLabel(this,
						"QuickUsb driver version");
  m_driverVersionLabel->setLabelStyle();
  m_driverVersionDisplay = new QLineEdit(this);
  m_driverVersionDisplay->setEnabled(false);
  m_driverVersionDisplay->setText(xpolgui::kNotAvailable);
  m_groupBoxGridLayout->addWidget(m_driverVersionLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_driverVersionDisplay, row, 1, 1, 2);
  row ++;
  m_dllVersionLabel = new pQtCustomTextLabel(this, "QuickUsb dll version");
  m_dllVersionLabel->setLabelStyle();
  m_dllVersionDisplay = new QLineEdit(this);
  m_dllVersionDisplay->setEnabled(false);
  m_dllVersionDisplay->setText(xpolgui::kNotAvailable);
  m_groupBoxGridLayout->addWidget(m_dllVersionLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_dllVersionDisplay, row, 1, 1, 2);
  row ++;
  m_firmwareVersionLabel = new pQtCustomTextLabel(this,
						  "Quickusb firmware version");
  m_firmwareVersionLabel->setLabelStyle();
  m_firmwareVersionDisplay = new QLineEdit(this);
  m_firmwareVersionDisplay->setEnabled(false);
  m_firmwareVersionDisplay->setText(xpolgui::kNotAvailable);
  m_groupBoxGridLayout->addWidget(m_firmwareVersionLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_firmwareVersionDisplay, row, 1, 1, 2);
  addVerticalSpacer();
  if (timeoutWidgets) {
    row += 2;
    m_timeoutLabel = new pQtCustomTextLabel(this, "USB timeout");
    m_timeoutLabel->setLabelStyle();
    m_timeoutSpinBox = new QSpinBox(this);
    m_timeoutSpinBox->setMinimum(1000);
    m_timeoutSpinBox->setMaximum(1000000);
    m_timeoutSpinBox->setSingleStep(1000);
    m_timeoutUnitsLabel = new pQtCustomTextLabel(this, "[ms]");
    m_groupBoxGridLayout->addWidget(m_timeoutLabel, row, 0);
    m_groupBoxGridLayout->addWidget(m_timeoutSpinBox, row, 1);
    m_groupBoxGridLayout->addWidget(m_timeoutUnitsLabel, row, 2);
  }
}


/*!
 */
void pUsbControlTab::updateInfo(QString deviceName, QString driverVersion,
				QString dllVersion, QString firmwareVersion)
{
  m_statusDisplay->setText("Connected");
  m_deviceNameDisplay->setText(deviceName);
  m_driverVersionDisplay->setText(driverVersion);
  m_dllVersionDisplay->setText(dllVersion);
  m_firmwareVersionDisplay->setText(firmwareVersion);
}
