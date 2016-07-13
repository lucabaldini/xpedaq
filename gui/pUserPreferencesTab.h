/***********************************************************************
Copyright (C) 2007, 2008 by Luca Baldini (luca.baldini@pi.infn.it),
Johan Bregeon, Massimo Minuti and Gloria Spandre.

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

#ifndef PUSERPREFERENCESTAB_H
#define PUSERPREFERENCESTAB_H

#include <iostream>
#include <math.h>
#include <QWidget>
#include <QString>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include "pQtCustomTextLabel.h"
#include "pQtCustomTab.h"
#include "pUserPreferences.h"
#include "pLogger.h"


class pUserPreferencesTab : public pQtCustomTab
{
  
  Q_OBJECT

 public:
  pUserPreferencesTab();
  ~pUserPreferencesTab() {;}
  pUserPreferences *getUserPreferences();
  int getVisualizationMode();
  bool dataFileEnabled();
  std::string getOutputFolder();
  bool multicastEnabled();
  std::string getMulticastAddress();
  int getMulticastPort();
  int getLoggerTerminalLevel();
  int getLoggerDisplayLevel();
  bool logFileEnabled();
  QString outputRootFolder();
  void displayUserPreferences(pUserPreferences preferences);

 public slots:
  void enableMulticastCheckBox(int enable = 1);
  void enableMulticastWidgets(int enable = 1);
  void enableOutputFolderWidgets(int enable = 1);
  void changeVisualizationMode();

 signals:
  void visualizetionModeChanged(int mode);

 protected:

 private:
  QString m_loggerDebugString;
  QString m_loggerInfoString;
  QString m_loggerWarningString;
  QString m_loggerErrorString;
  QRadioButton *m_daqVisualizationButton;
  QRadioButton *m_voltageVisualizationButton;
  QRadioButton *m_chargeVisualizationButton;
  QCheckBox *m_enableDataFileCheckBox;
  QCheckBox *m_enableMulticastCheckBox;
  pQtCustomTextLabel *m_outputFolderLabel;
  QLineEdit *m_outputFolderDisplay;
  pQtCustomTextLabel *m_multicastAddressLabel;
  QLineEdit *m_multicastAddressDisplay;
  pQtCustomTextLabel *m_multicastPortLabel;
  QLineEdit *m_multicastPortDisplay;
  QCheckBox *m_enableLogFileCheckBox;
  pQtCustomTextLabel *m_loggerTerminalLevelLabel;
  QComboBox *m_loggerTerminalLevelComboBox;
  pQtCustomTextLabel *m_loggerDisplayLevelLabel;
  QComboBox *m_loggerDisplayLevelComboBox;
  void setupVisualizationModeWidgets();
  void setupDataFileWidgets();
  void setupMulticastWidgets();
  void setupLoggerWidgets();
  void setupConnections();
  void setOutputFodler(std::string path);
  void setMulticastAddress(std::string address);
  void setMulticastPort(int port);
  int convertLoggerLevel(QString level);
  QString convertLoggerLevel(int level);
};

#endif
