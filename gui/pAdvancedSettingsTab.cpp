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

#include "pAdvancedSettingsTab.h"

pAdvancedSettingsTab::pAdvancedSettingsTab()
  : pQtCustomTab("Advanced settings")
{
  m_clockFasterLabel = "10 MHz";
  m_clockFastLabel = "5 MHz";
  m_clockSlowLabel = "2.5 MHz";
  m_clockSlowerLabel = "1.25 MHz";
  m_subSamplesZeroLabel = "0";
  m_subSamplesSmallerLabel = "1";
  m_subSamplesSmallLabel = "2";
  m_subSamplesLargeLabel = "4";
  m_subSamplesLargerLabel = "8";
  setupTimingWidgets();
  addVerticalSpacer();
  setupPedSubWidgets();
  addVerticalSpacer();
  setupWindowWidgets();
  freezeSize(xpolgui::kTabGroupBoxWidth);
}

void pAdvancedSettingsTab::setupTimingWidgets()
{
  int row = m_groupBoxGridLayout->rowCount();
  m_clockFrequencyLabel = new pQtCustomTextLabel(this, "Clock frequency");
  m_clockFrequencyComboBox = new QComboBox(this);
  m_clockFrequencyComboBox->insertItem(0, m_clockFasterLabel);
  m_clockFrequencyComboBox->insertItem(1, m_clockFastLabel);
  m_clockFrequencyComboBox->insertItem(2, m_clockSlowLabel);
  m_clockFrequencyComboBox->insertItem(3, m_clockSlowerLabel);
  m_clockFrequencyUnitsLabel = new pQtCustomTextLabel(this,
						      xpolgui::kMHzUnitsLabel);
  m_groupBoxGridLayout->addWidget(m_clockFrequencyLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_clockFrequencyComboBox, row, 1);
  m_groupBoxGridLayout->addWidget(m_clockFrequencyUnitsLabel, row, 2);
  row ++;
  m_clockShiftLabel = new pQtCustomTextLabel(this, "Clock shift");
  m_clockShiftSpinBox = new QSpinBox(this);
  m_clockShiftSpinBox->setMinimum(xpoldetector::kClockShiftMin);
  m_clockShiftSpinBox->setMaximum(xpoldetector::kClockShiftMax);
  m_clockShiftSpinBox->setSingleStep(xpoldetector::kClockShiftStep);
  m_clockShiftUnitsLabel = new pQtCustomTextLabel(this,
						  xpolgui::knsUnitsLabel);
  m_groupBoxGridLayout->addWidget(m_clockShiftLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_clockShiftSpinBox, row, 1);
  m_groupBoxGridLayout->addWidget(m_clockShiftUnitsLabel, row, 2);
}

void pAdvancedSettingsTab::setupPedSubWidgets()
{
  int row = m_groupBoxGridLayout->rowCount();
  m_numPedSamplesLabel =
    new pQtCustomTextLabel(this, "Number of samples for pedestal subtraction");
  m_numPedSamplesComboBox = new QComboBox(this);
  m_numPedSamplesComboBox->insertItem(0, m_subSamplesZeroLabel);
  m_numPedSamplesComboBox->insertItem(1, m_subSamplesSmallerLabel);
  m_numPedSamplesComboBox->insertItem(2, m_subSamplesSmallLabel);
  m_numPedSamplesComboBox->insertItem(3, m_subSamplesLargeLabel);
  m_numPedSamplesComboBox->insertItem(4, m_subSamplesLargerLabel);
  m_groupBoxGridLayout->addWidget(m_numPedSamplesLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_numPedSamplesComboBox, row, 1);
  row ++;
  m_pedSubDelayLabel =
    new pQtCustomTextLabel(this, "Sampling delay for pedestal subtraction");
  m_pedSubDelaySpinBox = new QSpinBox(this);
  m_pedSubDelaySpinBox->setMinimum(xpoldetector::kPedSubDelayMin);
  m_pedSubDelaySpinBox->setMaximum(xpoldetector::kPedSubDelayMax);
  m_pedSubDelaySpinBox->setSingleStep(xpoldetector::kPedSubDelayStep);
  m_pedSubDelayUnitsLabel = new pQtCustomTextLabel(this,
						   xpolgui::kusUnitsLabel);
  m_groupBoxGridLayout->addWidget(m_pedSubDelayLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_pedSubDelaySpinBox, row, 1);
  m_groupBoxGridLayout->addWidget(m_pedSubDelayUnitsLabel, row, 2);
  row ++;
  m_trgEnableDelayLabel =
    new pQtCustomTextLabel(this, "Trigger enable delay after analog reset");
  m_trgEnableDelaySpinBox = new QSpinBox(this);
  m_trgEnableDelaySpinBox->setMinimum(xpoldetector::kTrgEnableDelayMin);
  m_trgEnableDelaySpinBox->setMaximum(xpoldetector::kTrgEnableDelayMax);
  m_trgEnableDelaySpinBox->setSingleStep(xpoldetector::kTrgEnableDelayStep);
  m_trgEnableDelayUnitsLabel = new pQtCustomTextLabel(this,
						   xpolgui::kusUnitsLabel);
  m_groupBoxGridLayout->addWidget(m_trgEnableDelayLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_trgEnableDelaySpinBox, row, 1);
  m_groupBoxGridLayout->addWidget(m_trgEnableDelayUnitsLabel, row, 2);
}

void pAdvancedSettingsTab::setupWindowWidgets()
{
  int row = m_groupBoxGridLayout->rowCount();
  m_minWindowSizeLabel =
    new pQtCustomTextLabel(this, "Minimum window dimension");
  m_minWindowSizeSpinBox = new QSpinBox(this);
  m_minWindowSizeSpinBox->setMinimum(xpoldetector::kMinWindowSizeMin);
  m_minWindowSizeSpinBox->setMaximum(xpoldetector::kMinWindowSizeMax);
  m_minWindowSizeSpinBox->setSingleStep(xpoldetector::kMinWindowSizeStep);
  m_groupBoxGridLayout->addWidget(m_minWindowSizeLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_minWindowSizeSpinBox, row, 1);
  row ++;
  m_maxWindowSizeLabel =
    new pQtCustomTextLabel(this, "Maximum window dimension");
  m_maxWindowSizeSpinBox = new QSpinBox(this);
  m_maxWindowSizeSpinBox->setMinimum(xpoldetector::kMaxWindowSizeMin);
  m_maxWindowSizeSpinBox->setMaximum(xpoldetector::kMaxWindowSizeMax);
  m_maxWindowSizeSpinBox->setSingleStep(xpoldetector::kMaxWindowSizeStep);
  m_groupBoxGridLayout->addWidget(m_maxWindowSizeLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_maxWindowSizeSpinBox, row, 1);
}

unsigned short int pAdvancedSettingsTab::getTimingCode()
{
  int clockFreqCode;
  int clockShiftCode;
  QString clockFreqString = m_clockFrequencyComboBox->currentText();
  if (clockFreqString == m_clockFasterLabel){
    clockFreqCode = xpoldetector::kClockFasterFreqCode;
  } else if (clockFreqString == m_clockFastLabel){
    clockFreqCode = xpoldetector::kClockFastFreqCode;
  } else if (clockFreqString == m_clockSlowLabel){
    clockFreqCode = xpoldetector::kClockSlowFreqCode;
  } else if (clockFreqString == m_clockSlowerLabel){
    clockFreqCode = xpoldetector::kClockSlowerFreqCode;
  } else {
    // Put a warning here;
    clockFreqCode = xpoldetector::kClockFasterFreqCode;
  }
  clockShiftCode = m_clockShiftSpinBox->value()/xpoldetector::kClockShiftStep;
  return clockFreqCode | (clockShiftCode & 0x1f);
}

void pAdvancedSettingsTab::displayTimingCode(unsigned short int code)
{
  int clockFreqCode = (code & 0xe0);
  int clockShiftCode = (code & 0x1f);
  if (clockFreqCode == xpoldetector::kClockFasterFreqCode){
    m_clockFrequencyComboBox->setCurrentIndex(0);
  } else if (clockFreqCode == xpoldetector::kClockFastFreqCode){
    m_clockFrequencyComboBox->setCurrentIndex(1);
  } else if (clockFreqCode == xpoldetector::kClockSlowFreqCode){
    m_clockFrequencyComboBox->setCurrentIndex(2);
  } else if (clockFreqCode == xpoldetector::kClockSlowerFreqCode){
    m_clockFrequencyComboBox->setCurrentIndex(3);
  } else {
    //Put a warning here;
    m_clockFrequencyComboBox->setCurrentIndex(0);
  }
  m_clockShiftSpinBox->setValue(clockShiftCode*xpoldetector::kClockShiftStep);
}

unsigned short int pAdvancedSettingsTab::getNumPedSamples()
{
  QString numPedSamplesString = m_numPedSamplesComboBox->currentText();
  if (numPedSamplesString == m_subSamplesZeroLabel){
    return xpoldetector::kSubSamplesZero;
  } else if (numPedSamplesString == m_subSamplesSmallerLabel){
    return xpoldetector::kSubSamplesSmaller;
  } else if (numPedSamplesString == m_subSamplesSmallLabel){
    return xpoldetector::kSubSamplesSmall;
  } else if (numPedSamplesString == m_subSamplesLargeLabel){
    return xpoldetector::kSubSamplesLarge;
  } else if (numPedSamplesString == m_subSamplesLargerLabel){
    return xpoldetector::kSubSamplesLarger;
  } else {
    //Put a warning here;
    return xpoldetector::kSubSamplesZero;
  }
}

void pAdvancedSettingsTab::displayNumPedsSamples(unsigned short int samples)
{
  if (samples == xpoldetector::kSubSamplesZero){
    m_numPedSamplesComboBox->setCurrentIndex(0);
  } else if (samples == xpoldetector::kSubSamplesSmaller){
    m_numPedSamplesComboBox->setCurrentIndex(1);
  } else if (samples == xpoldetector::kSubSamplesSmall){
    m_numPedSamplesComboBox->setCurrentIndex(2);
  } else if (samples == xpoldetector::kSubSamplesLarge){
    m_numPedSamplesComboBox->setCurrentIndex(3);
  } else if (samples == xpoldetector::kSubSamplesLarger){
    m_numPedSamplesComboBox->setCurrentIndex(4);
  } else {
    //Put a warning here;
    m_numPedSamplesComboBox->setCurrentIndex(0);
  }
}

unsigned short int pAdvancedSettingsTab::getPedSubDelay()
{
  return m_pedSubDelaySpinBox->value()/xpoldetector::kPedSubDelayStep;
}

void pAdvancedSettingsTab::displayPedSubDelay(unsigned short int delay)
{
  m_pedSubDelaySpinBox->setValue(delay*xpoldetector::kPedSubDelayStep);
}

unsigned short int pAdvancedSettingsTab::getTrgEnableDelay()
{
  return m_trgEnableDelaySpinBox->value()/xpoldetector::kTrgEnableDelayStep;
}

void pAdvancedSettingsTab::displayTrgEnableDelay(unsigned short int delay)
{
  m_trgEnableDelaySpinBox->setValue(delay*xpoldetector::kTrgEnableDelayStep);
}

unsigned short int pAdvancedSettingsTab::getMinWindowSize()
{
  return m_minWindowSizeSpinBox->value()/xpoldetector::kMinWindowSizeStep;
}

void pAdvancedSettingsTab::displayMinWindowSize(unsigned short int size)
{
  m_minWindowSizeSpinBox->setValue(size*xpoldetector::kMinWindowSizeStep);
}

unsigned short int pAdvancedSettingsTab::getMaxWindowSize()
{
  return m_maxWindowSizeSpinBox->value()/xpoldetector::kMaxWindowSizeStep;
}

void pAdvancedSettingsTab::displayMaxWindowSize(unsigned short int size)
{
  m_maxWindowSizeSpinBox->setValue(size*xpoldetector::kMaxWindowSizeStep);
}

void pAdvancedSettingsTab::displayConfiguration(pDetectorConfiguration
						*configuration)
{
  displayTimingCode(configuration->getTimingCode());
  displayNumPedsSamples(configuration->getNumPedSamples());
  displayPedSubDelay(configuration->getPedSampleDelay());
  displayTrgEnableDelay(configuration->getTrgEnableDelay());
  displayMinWindowSize(configuration->getMinWindowSize());
  displayMaxWindowSize(configuration->getMaxWindowSize());
}
