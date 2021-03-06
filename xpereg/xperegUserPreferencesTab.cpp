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


#include "xperegUserPreferencesTab.h"


xperegUserPreferencesTab::xperegUserPreferencesTab() :
  pUserPreferencesTab(false)
{

}



void xperegUserPreferencesTab::displayUserPreferences(xperegUserPreferences
						      *preferences)
{
  m_enableDataFileCheckBox->setChecked(preferences->m_dataFileEnabled);
  setOutputFolder(preferences->m_outputFolder);
  m_loggerTerminalLevelComboBox->
    setCurrentIndex(preferences->m_loggerTerminalLevel);
  m_loggerDisplayLevelComboBox->
    setCurrentIndex(preferences->m_loggerDisplayLevel);
  m_enableLogFileCheckBox->setChecked(preferences->m_logFileEnabled);  
}


void xperegUserPreferencesTab::userPreferences(xperegUserPreferences
					       &preferences)
{
  preferences.m_dataFileEnabled = dataFileEnabled();
  preferences.m_outputFolder = getOutputFolder();
  preferences.m_logFileEnabled = logFileEnabled();
  preferences.m_loggerTerminalLevel = getLoggerTerminalLevel();
  preferences.m_loggerDisplayLevel = getLoggerDisplayLevel();
}

 
