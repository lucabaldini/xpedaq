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

#include "pAdvancedSettingsTab.h"


/*!
 */
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


/*!
 */
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


/*!
 */
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


/*!
 */
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


/*!
 */
unsigned short pAdvancedSettingsTab::clockFrequency() const
{
  QString clockFreqString = m_clockFrequencyComboBox->currentText();
  if (clockFreqString == m_clockFasterLabel){
    return xpoldetector::kClockFasterFreqCode;
  } else if (clockFreqString == m_clockFastLabel){
    return xpoldetector::kClockFastFreqCode;
  } else if (clockFreqString == m_clockSlowLabel){
    return xpoldetector::kClockSlowFreqCode;
  } else if (clockFreqString == m_clockSlowerLabel){
    return xpoldetector::kClockSlowerFreqCode;
  } else {
    return xpoldetector::kClockFastFreqCode;
  }
}


/*!
 */
unsigned short pAdvancedSettingsTab::clockShift() const
{
  return m_clockShiftSpinBox->value()/xpoldetector::kClockShiftStep;
}


/*!
 */
unsigned short pAdvancedSettingsTab::numPedSamples() const
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
    return xpoldetector::kSubSamplesZero;
  }
}


/*!
 */
unsigned short pAdvancedSettingsTab::pedSubDelay() const
{
  return m_pedSubDelaySpinBox->value()/xpoldetector::kPedSubDelayStep;
}


/*!
 */
unsigned short pAdvancedSettingsTab::trgEnableDelay() const
{
  return m_trgEnableDelaySpinBox->value()/xpoldetector::kTrgEnableDelayStep;
}


/*!
 */
unsigned short pAdvancedSettingsTab::minWindowSize() const
{
  return m_minWindowSizeSpinBox->value()/xpoldetector::kMinWindowSizeStep;
}


/*!
 */
unsigned short pAdvancedSettingsTab::maxWindowSize() const
{
  return m_maxWindowSizeSpinBox->value()/xpoldetector::kMaxWindowSizeStep;
}


/*!
 */
void pAdvancedSettingsTab::displayClockFrequency(unsigned short frequency)
{
  if (frequency == xpoldetector::kClockFasterFreqCode){
    m_clockFrequencyComboBox->setCurrentIndex(0);
  } else if (frequency == xpoldetector::kClockFastFreqCode){
    m_clockFrequencyComboBox->setCurrentIndex(1);
  } else if (frequency == xpoldetector::kClockSlowFreqCode){
    m_clockFrequencyComboBox->setCurrentIndex(2);
  } else if (frequency == xpoldetector::kClockSlowerFreqCode){
    m_clockFrequencyComboBox->setCurrentIndex(3);
  } else {
    m_clockFrequencyComboBox->setCurrentIndex(1);
  }
}


/*!
 */
void pAdvancedSettingsTab::displayClockShift(unsigned short shift)
{
  m_clockShiftSpinBox->setValue(shift*xpoldetector::kClockShiftStep);
}


/*!
 */
void pAdvancedSettingsTab::displayNumPedsSamples(unsigned short samples)
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


/*!
 */
void pAdvancedSettingsTab::displayPedSubDelay(unsigned short delay)
{
  m_pedSubDelaySpinBox->setValue(delay*xpoldetector::kPedSubDelayStep);
}


/*!
 */
void pAdvancedSettingsTab::displayTrgEnableDelay(unsigned short delay)
{
  m_trgEnableDelaySpinBox->setValue(delay*xpoldetector::kTrgEnableDelayStep);
}


/*!
 */
void pAdvancedSettingsTab::displayMinWindowSize(unsigned short size)
{
  m_minWindowSizeSpinBox->setValue(size*xpoldetector::kMinWindowSizeStep);
}


/*!
 */
void pAdvancedSettingsTab::displayMaxWindowSize(unsigned short size)
{
  m_maxWindowSizeSpinBox->setValue(size*xpoldetector::kMaxWindowSizeStep);
}


/*!
 */
void pAdvancedSettingsTab::displayConfiguration(pDetectorConfiguration
						*configuration)
{
  displayClockFrequency(configuration->clockFrequency());
  displayClockShift(configuration->clockShift());
  displayNumPedsSamples(configuration->numPedSamples());
  displayPedSubDelay(configuration->pedSampleDelay());
  displayTrgEnableDelay(configuration->trgEnableDelay());
  displayMinWindowSize(configuration->minWindowSize());
  displayMaxWindowSize(configuration->maxWindowSize());
}


/*!
 */
void pAdvancedSettingsTab::disableNumPedSamplesComboBox(bool disable)
{
  m_numPedSamplesComboBox -> setDisabled(disable);
}


/*!
 */
void pAdvancedSettingsTab::disableMinWindowSizeSpinBox(bool disable)
{
  m_minWindowSizeSpinBox -> setDisabled(disable);
}


/*!
 */
void pAdvancedSettingsTab::disableMaxWindowSizeSpinBox(bool disable)
{
  m_maxWindowSizeSpinBox -> setDisabled(disable);
}
