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

#ifndef PUSBCONTROLTAB_H
#define PUSBCONTROLTAB_H

#include <iostream>

#include <QString>
#include <QLineEdit>
#include <QSpinBox>

#include "pQtCustomTextLabel.h"
#include "pQtCustomTab.h"

class pUsbControlTab : public pQtCustomTab
{

  Q_OBJECT

 public:

  pUsbControlTab(bool timeoutWidgets = true);
  ~pUsbControlTab() {;}
  int timeout() const;
  
  
 public slots:

   void updateInfo(QString deviceName, QString driverVersion,
		   QString dllVersion, QString firmwareVersion);
   void setTimeout(int timeout);
   

 private:

  void setupWidgets(bool timeoutWidgets);
  pQtCustomTextLabel *m_statusLabel;
  QLineEdit *m_statusDisplay;
  pQtCustomTextLabel *m_deviceNameLabel;
  QLineEdit *m_deviceNameDisplay;
  pQtCustomTextLabel *m_driverVersionLabel;
  QLineEdit *m_driverVersionDisplay;
  pQtCustomTextLabel *m_dllVersionLabel;
  QLineEdit *m_dllVersionDisplay;
  pQtCustomTextLabel *m_firmwareVersionLabel;
  QLineEdit *m_firmwareVersionDisplay;
  pQtCustomTextLabel *m_timeoutLabel;
  QSpinBox *m_timeoutSpinBox;
  pQtCustomTextLabel *m_timeoutUnitsLabel;
};

#endif //PUSBCONTROLTAB_H
