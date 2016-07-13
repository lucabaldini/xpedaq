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

/*!
  \todo Implement a push button to apply the user preferences.
*/

#include "pUserPreferencesTab.h"

pUserPreferencesTab::pUserPreferencesTab()
  : pQtCustomTab("Preferences")
{
  m_groupBoxGridLayout->setColumnMinimumWidth(0, 250);
  m_loggerDebugString = "DEBUG";
  m_loggerInfoString = "INFO";
  m_loggerWarningString = "WARNING";
  m_loggerErrorString = "ERROR";
  setupVisualizationModeWidgets();
  addVerticalSpacer();
  setupDataFileWidgets();
  addVerticalSpacer();
  setupMulticastWidgets();
  addVerticalSpacer();
  setupLoggerWidgets();
  freezeSize(xpolgui::kTabGroupBoxWidth);
  setupConnections();
  // To be uncommented when the functionality is implemented.
  m_enableDataFileCheckBox->setEnabled(false);
  enableOutputFolderWidgets(false);
}

QString pUserPreferencesTab::outputRootFolder()
{
  return m_outputFolderDisplay->text();
}

void pUserPreferencesTab::setupVisualizationModeWidgets()
{
  int row = m_groupBoxGridLayout->rowCount();
  m_daqVisualizationButton =
    new QRadioButton("Display quantities in DAC (raw values)");
  m_voltageVisualizationButton =
    new QRadioButton("Display quantities in voltage (Volt)");
  m_chargeVisualizationButton =
    new QRadioButton("Display quantities in charge (electrons)");
  m_groupBoxGridLayout->addWidget(m_daqVisualizationButton, row, 0, 1, 2);
  row ++;
  m_groupBoxGridLayout->addWidget(m_voltageVisualizationButton, row, 0, 1, 2);
  row ++;
  m_groupBoxGridLayout->addWidget(m_chargeVisualizationButton, row, 0, 1, 2);
}

void pUserPreferencesTab::setupDataFileWidgets()
{
  int row = m_groupBoxGridLayout->rowCount();
  m_enableDataFileCheckBox = new QCheckBox("Write data to disk", this);
  m_groupBoxGridLayout->addWidget(m_enableDataFileCheckBox, row, 0);
  row++;
  m_outputFolderLabel = new pQtCustomTextLabel(this, "Output root folder");
  m_outputFolderDisplay = new QLineEdit(this);
  m_outputFolderLabel->setLabelStyle();
  m_groupBoxGridLayout->addWidget(m_outputFolderLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_outputFolderDisplay, row, 1);
  row ++;
}

void pUserPreferencesTab::setupMulticastWidgets()
{
  int row = m_groupBoxGridLayout->rowCount();
  m_enableMulticastCheckBox = new QCheckBox("Enable multicast", this);
  m_groupBoxGridLayout->addWidget(m_enableMulticastCheckBox, row, 0);
  row ++;
  m_multicastAddressLabel = new pQtCustomTextLabel(this, "Multicast address");
  m_multicastAddressDisplay = new QLineEdit(this);
  m_multicastAddressLabel->setLabelStyle();
  m_groupBoxGridLayout->addWidget(m_multicastAddressLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_multicastAddressDisplay, row, 1);
  row ++;
  m_multicastPortLabel = new pQtCustomTextLabel(this, "Multicast port");
  m_multicastPortDisplay = new QLineEdit(this);
  m_multicastPortLabel->setLabelStyle();
  m_groupBoxGridLayout->addWidget(m_multicastPortLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_multicastPortDisplay, row, 1);
}

void pUserPreferencesTab::setupLoggerWidgets()
{
  int row = m_groupBoxGridLayout->rowCount();
  m_enableLogFileCheckBox = new QCheckBox("Write log file", this);
  m_groupBoxGridLayout->addWidget(m_enableLogFileCheckBox, row, 0);
  row ++;
  m_loggerTerminalLevelLabel =
    new pQtCustomTextLabel(this, "Logger level (terminal)");
  m_loggerTerminalLevelComboBox = new QComboBox(this);
  m_loggerTerminalLevelComboBox->insertItem(pLogger::Debug,
					    m_loggerDebugString);
  m_loggerTerminalLevelComboBox->insertItem(pLogger::Info,
					    m_loggerInfoString);
  m_loggerTerminalLevelComboBox->insertItem(pLogger::Warning,
					    m_loggerWarningString);
  m_loggerTerminalLevelComboBox->insertItem(pLogger::Error,
					    m_loggerErrorString);
  m_groupBoxGridLayout->addWidget(m_loggerTerminalLevelLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_loggerTerminalLevelComboBox, row, 1);
  row ++;
  m_loggerDisplayLevelLabel =
    new pQtCustomTextLabel(this, "Logger level (message display)");
  m_loggerDisplayLevelComboBox = new QComboBox(this);
  m_loggerDisplayLevelComboBox->insertItem(pLogger::Debug,
					   m_loggerDebugString);
  m_loggerDisplayLevelComboBox->insertItem(pLogger::Info,
					   m_loggerInfoString);
  m_loggerDisplayLevelComboBox->insertItem(pLogger::Warning,
					   m_loggerWarningString);
  m_loggerDisplayLevelComboBox->insertItem(pLogger::Error,
					   m_loggerErrorString);
  m_groupBoxGridLayout->addWidget(m_loggerDisplayLevelLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_loggerDisplayLevelComboBox, row, 1);
}

void pUserPreferencesTab::enableOutputFolderWidgets(int enable)
{
  m_outputFolderLabel->setEnabled(enable);
  m_outputFolderDisplay->setEnabled(enable);
}

void pUserPreferencesTab::enableMulticastCheckBox(int enable)
{
  m_enableMulticastCheckBox->setEnabled(enable);
}

void pUserPreferencesTab::enableMulticastWidgets(int enable)
{
  m_multicastAddressLabel->setEnabled(enable);
  m_multicastAddressDisplay->setEnabled(enable);
  m_multicastPortLabel->setEnabled(enable);
  m_multicastPortDisplay->setEnabled(enable);
}

void pUserPreferencesTab::setupConnections()
{
  //connect(m_enableDataFileCheckBox, SIGNAL(stateChanged(int)),
  //this, SLOT(enableOutputFolderWidgets(int)));
  connect(m_enableMulticastCheckBox, SIGNAL(stateChanged(int)),
	  this, SLOT(enableMulticastWidgets(int)));
  connect(m_daqVisualizationButton, SIGNAL(clicked()),
	  this, SLOT(changeVisualizationMode()));
  connect(m_voltageVisualizationButton, SIGNAL(clicked()),
	  this, SLOT(changeVisualizationMode()));
  connect(m_chargeVisualizationButton, SIGNAL(clicked()),
	  this, SLOT(changeVisualizationMode()));
}

int pUserPreferencesTab::getVisualizationMode()
{
  if (m_daqVisualizationButton->isChecked()){
    return xpolgui::kDacVisualizationCode;
  } else if (m_voltageVisualizationButton->isChecked()){
    return xpolgui::kVoltageVisualizationCode;
  } else if (m_chargeVisualizationButton->isChecked()){
    return xpolgui::kChargeVisualizationCode;
  } else {
    return xpolgui::kUndefinedVisualizationCode;
  }
}

std::string pUserPreferencesTab::getOutputFolder()
{
  return m_outputFolderDisplay->text().toStdString();
}

bool pUserPreferencesTab::multicastEnabled()
{
  return m_enableMulticastCheckBox->isChecked();
}

std::string pUserPreferencesTab::getMulticastAddress()
{
  return m_multicastAddressDisplay->text().toStdString();
}

int pUserPreferencesTab::getMulticastPort()
{
  return m_multicastPortDisplay->text().toInt();
}

int pUserPreferencesTab::getLoggerTerminalLevel()
{
  return convertLoggerLevel(m_loggerTerminalLevelComboBox->currentText());
}

int pUserPreferencesTab::getLoggerDisplayLevel()
{
  return convertLoggerLevel(m_loggerDisplayLevelComboBox->currentText());
}

bool pUserPreferencesTab::dataFileEnabled()
{
  return m_enableDataFileCheckBox->isChecked();
}

bool pUserPreferencesTab::logFileEnabled()
{
  return m_enableLogFileCheckBox->isChecked();
}

pUserPreferences *pUserPreferencesTab::getUserPreferences()
{
  pUserPreferences *preferences = new pUserPreferences();
  preferences->setVisualizationMode(getVisualizationMode());
  preferences->enableDataFile(dataFileEnabled());
  preferences->setOutputFolder(getOutputFolder());
  preferences->enableMulticast(multicastEnabled());
  preferences->setMulticastAddress(getMulticastAddress());
  preferences->setMulticastPort(getMulticastPort());
  preferences->setLoggerTerminalLevel(getLoggerTerminalLevel());
  preferences->setLoggerDisplayLevel(getLoggerDisplayLevel());
  preferences->enableLogFile(logFileEnabled());
  return preferences;
}

void pUserPreferencesTab::displayUserPreferences(pUserPreferences preferences)
{
  if (preferences.visualizationMode() == xpolgui::kDacVisualizationCode) {
    m_daqVisualizationButton->setChecked(true);
  } else if (preferences.visualizationMode() == xpolgui::kVoltageVisualizationCode) {
    m_voltageVisualizationButton->setChecked(true);
  } else if (preferences.visualizationMode() == xpolgui::kChargeVisualizationCode){
    m_chargeVisualizationButton->setChecked(true);
  } else {
    *xpollog::kError << "Invalid display." << endline;
  }
  m_enableDataFileCheckBox->setChecked(preferences.dataFileEnabled());
  setOutputFodler(preferences.outputFolder());
  m_enableMulticastCheckBox->setChecked(preferences.multicastEnabled());
  enableMulticastWidgets(preferences.multicastEnabled());
  setMulticastAddress(preferences.multicastAddress());
  setMulticastPort(preferences.multicastPort());
  m_loggerTerminalLevelComboBox->
    setCurrentIndex(preferences.loggerTerminalLevel());
  m_loggerDisplayLevelComboBox->
    setCurrentIndex(preferences.loggerDisplayLevel());
  m_enableLogFileCheckBox->setChecked(preferences.logFileEnabled());
}

void pUserPreferencesTab::setOutputFodler(std::string path)
{
  QString pathQString = QString(path.c_str());
  m_outputFolderDisplay->setText(pathQString);
}

void pUserPreferencesTab::setMulticastAddress(std::string address)
{
  QString addressQString = QString(address.c_str());
  m_multicastAddressDisplay->setText(addressQString);
}

void pUserPreferencesTab::setMulticastPort(int port)
{
  QString portQString = QString::number(port);
  m_multicastPortDisplay->setText(portQString);
}

void pUserPreferencesTab::changeVisualizationMode()
{
  emit visualizetionModeChanged(getVisualizationMode());  
}

int pUserPreferencesTab::convertLoggerLevel(QString level)
{
  if (level == m_loggerDebugString)
    {
      return pLogger::Debug;
    }
  else if (level == m_loggerInfoString)
    {
      return pLogger::Info;
    }
  else if (level == m_loggerWarningString)
    {
      return pLogger::Warning;
    }
  else if (level == m_loggerErrorString)
    {
      return pLogger::Error;
    }
  else
    {
      return pLogger::Debug;
    }
}

QString pUserPreferencesTab::convertLoggerLevel(int level)
{
  if (level == pLogger::Debug)
    {
      return m_loggerDebugString;
    }
  else if (level == pLogger::Info)
    {
      return m_loggerInfoString;
    }
  else if (level == pLogger::Warning)
    {
      return m_loggerWarningString;
    }
  else if (level == pLogger::Error)
    {
      return m_loggerErrorString;
    }
  else
    {
      return m_loggerDebugString;
    }
}
