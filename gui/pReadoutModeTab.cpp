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

#include "pReadoutModeTab.h"

pReadoutModeTab::pReadoutModeTab()
  : pQtCustomTab("Readout mode")
{
  setupReadoutModeWidgets();
  addVerticalSpacer();
  setupBufferModeWidgets();
  addVerticalSpacer();
  setupCalibrationWidgets();
  setupConnections();
  freezeSize(xpolgui::kTabGroupBoxWidth, -1);
}

void pReadoutModeTab::setupReadoutModeWidgets()
{
  m_fullFrameRadioButton = new QRadioButton("Full frame");
  m_windowRadioButton = new QRadioButton("Window mode");
  m_chargeInjectionRadioButton = new QRadioButton("Charge injection");
  m_groupBoxGridLayout->addWidget(m_fullFrameRadioButton, 0, 0);
  m_groupBoxGridLayout->addWidget(m_windowRadioButton, 1, 0);
  m_groupBoxGridLayout->addWidget(m_chargeInjectionRadioButton, 2, 0);
}

void pReadoutModeTab::setupBufferModeWidgets()
{
  m_smallBufferLabel = "Small";
  m_largeBufferLabel = "Large";
  int row = m_groupBoxGridLayout->rowCount();
  m_bufferModeLabel = new pQtCustomTextLabel(this, "Buffer size");
  m_bufferModeComboBox = new QComboBox(this);
  m_bufferModeComboBox->insertItem(0, m_smallBufferLabel);
  m_bufferModeComboBox->insertItem(1, m_largeBufferLabel);
  m_groupBoxGridLayout->addWidget(m_bufferModeLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_bufferModeComboBox, row, 1);
}

void pReadoutModeTab::setupCalibrationWidgets()
{
  int row = m_groupBoxGridLayout->rowCount();
  m_calibrationLabel = new pQtCustomTextLabel(this, "Calibration signal");
  m_calibrationSpinBox = new QSpinBox(this);
  m_calibrationUnitsLabel =
    new pQtCustomTextLabel(this, xpolgui::kUndefinedUnitsLabel);
  m_calibrationLabel->setLabelStyle();
  row ++;
  m_groupBoxGridLayout->addWidget(m_calibrationLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_calibrationSpinBox, row , 1);
  m_groupBoxGridLayout->addWidget(m_calibrationUnitsLabel, row, 3);
  m_groupBoxGridLayout->
    setColumnMinimumWidth(2, xpolgui::kTabColumnsSmallSpacing);
  m_groupBoxGridLayout->setColumnStretch(4, 1);
  row ++;
  m_calPixAddressXLabel = new pQtCustomTextLabel(this, "Pixel address x");
  m_calPixAddressXSpinBox = new QSpinBox(this);
  m_calPixAddressXSpinBox->setMinimum(0);
  m_calPixAddressXSpinBox->setMaximum(xpoldetector::kNumPixelsX);
  m_groupBoxGridLayout->addWidget(m_calPixAddressXLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_calPixAddressXSpinBox, row , 1);
  row ++;
  m_calPixAddressYLabel = new pQtCustomTextLabel(this, "Pixel address x");
  m_calPixAddressYSpinBox = new QSpinBox(this);
  m_calPixAddressYSpinBox->setMinimum(0);
  m_calPixAddressYSpinBox->setMaximum(xpoldetector::kNumPixelsY);
  m_groupBoxGridLayout->addWidget(m_calPixAddressYLabel, row, 0);
  m_groupBoxGridLayout->addWidget(m_calPixAddressYSpinBox, row , 1);
}

void pReadoutModeTab::setupConnections()
{
  connect(m_fullFrameRadioButton, SIGNAL(clicked(bool)),
	  this, SLOT(disableCalibrationWidgets(bool)));
  connect(m_windowRadioButton, SIGNAL(clicked(bool)),
	  this, SLOT(disableCalibrationWidgets(bool)));
  connect(m_chargeInjectionRadioButton, SIGNAL(clicked(bool)),
	  this, SLOT(enableCalibrationWidgets(bool)));
  connect(m_fullFrameRadioButton, SIGNAL(clicked(bool)),
	  this, SLOT(disableBufferModeWidgets(bool)));
  connect(m_windowRadioButton, SIGNAL(clicked(bool)),
	  this, SLOT(enableBufferModeWidgets(bool)));
  connect(m_chargeInjectionRadioButton, SIGNAL(clicked(bool)),
	  this, SLOT(enableBufferModeWidgets(bool)));
}

void pReadoutModeTab::enableCalibrationWidgets(bool enable)
{
  m_calibrationLabel->setEnabled(enable);
  m_calibrationSpinBox->setEnabled(enable);
  m_calibrationUnitsLabel->setEnabled(enable);
  m_calPixAddressXLabel->setEnabled(enable);
  m_calPixAddressXSpinBox->setEnabled(enable);
  m_calPixAddressYLabel->setEnabled(enable);
  m_calPixAddressYSpinBox->setEnabled(enable);
}

void pReadoutModeTab::disableCalibrationWidgets(bool disable)
{
  m_calibrationLabel->setDisabled(disable);
  m_calibrationSpinBox->setDisabled(disable);
  m_calibrationUnitsLabel->setDisabled(disable);
  m_calPixAddressXLabel->setDisabled(disable);
  m_calPixAddressXSpinBox->setDisabled(disable);
  m_calPixAddressYLabel->setDisabled(disable);
  m_calPixAddressYSpinBox->setDisabled(disable);
}

void pReadoutModeTab::enableBufferModeWidgets(bool enable)
{
  m_bufferModeLabel->setEnabled(enable);
  m_bufferModeComboBox->setEnabled(enable);
}

void pReadoutModeTab::disableBufferModeWidgets(bool disable)
{
  m_bufferModeLabel->setDisabled(disable);
  m_bufferModeComboBox->setDisabled(disable);
}

unsigned short int pReadoutModeTab::getReadoutMode()
{
  if (m_fullFrameRadioButton->isChecked()){
    return xpoldetector::kFullFrameReadoutCode;
  } else if (m_windowRadioButton->isChecked()){
    return xpoldetector::kWindowedReadoutCode;
  } else if (m_chargeInjectionRadioButton->isChecked()){
    return xpoldetector::kChargeInjectionReadoutCode;
  } else {
    return xpoldetector::kUndefinedReadoutCode;
  }
}

void pReadoutModeTab::displayReadoutMode(unsigned short int mode)
{
  if (mode == xpoldetector::kFullFrameReadoutCode){
    m_fullFrameRadioButton->setChecked(true);
    disableCalibrationWidgets();
    disableBufferModeWidgets();
  } else if (mode == xpoldetector::kWindowedReadoutCode){
    m_windowRadioButton->setChecked(true);
    disableCalibrationWidgets();
    enableBufferModeWidgets();
  } else if (mode == xpoldetector::kChargeInjectionReadoutCode){
    m_chargeInjectionRadioButton->setChecked(true);
    enableCalibrationWidgets();
    enableBufferModeWidgets();
  }
}

unsigned short int pReadoutModeTab::getBufferMode()
{
  QString bufferModeString = m_bufferModeComboBox->currentText();
  if (bufferModeString == m_smallBufferLabel){
    return xpoldetector::kSmallBufferMode;
  } else if (bufferModeString == m_largeBufferLabel){
    return xpoldetector::kLargeBufferMode;
  } else {
    // Put a warning here;
    return xpoldetector::kSmallBufferMode;
  }
}

void pReadoutModeTab::displayBufferMode(unsigned short int mode)
{
  if (mode == xpoldetector::kSmallBufferMode){
    m_bufferModeComboBox->setCurrentIndex(0);
  } else if (mode == xpoldetector::kLargeBufferMode){
    m_bufferModeComboBox->setCurrentIndex(1);
  } else {
    // Put a warning here;
    m_bufferModeComboBox->setCurrentIndex(0);
  }
}

unsigned short int pReadoutModeTab::getCalibrationSignal(int visualizationMode)
{
  short unsigned int calibration = m_calibrationSpinBox->value();
  if (visualizationMode == xpolgui::kDacVisualizationCode){
    return calibration;
  } else if (visualizationMode == xpolgui::kChargeVisualizationCode){
    return xpoldetector::calChargeToDac(calibration);
  } else if (visualizationMode == xpolgui::kVoltageVisualizationCode){
    return xpoldetector::calVoltageToDac(calibration);
  } else {
    *xpollog::kWarning << "Undefined visualization mode." << endline;
    return 0;
  } 
}

void pReadoutModeTab::displayCalibrationSignal(unsigned short int calSignal,
					       int visualizationMode)
{
  if (visualizationMode == xpolgui::kDacVisualizationCode){
    m_calibrationSpinBox->setMinimum(xpoldetector::kCalibrationDacMin);
    m_calibrationSpinBox->setMaximum(xpoldetector::kCalibrationDacMax);
    m_calibrationSpinBox->setValue(calSignal);
    m_calibrationUnitsLabel->setText(xpolgui::kDACUnitsLabel);
  } else if (visualizationMode == xpolgui::kChargeVisualizationCode){
    m_calibrationSpinBox->setMinimum(xpoldetector::kCalibrationChargeMin);
    m_calibrationSpinBox->setMaximum(xpoldetector::kCalibrationChargeMax);
    m_calibrationSpinBox->setValue(xpoldetector::calDacToCharge(calSignal));
    m_calibrationUnitsLabel->setText(xpolgui::kChargeUnitsLabel);
  } else if (visualizationMode == xpolgui::kVoltageVisualizationCode){
    m_calibrationSpinBox->setMinimum(xpoldetector::kCalibrationVoltageMin);
    m_calibrationSpinBox->setMaximum(xpoldetector::kCalibrationVoltageMax);
    m_calibrationSpinBox->setValue(xpoldetector::calDacToVoltage(calSignal));
    m_calibrationUnitsLabel->setText(xpolgui::kVoltageUnitsLabel);
  } else {
    *xpollog::kWarning << "Undefined visualization mode." << endline;
    m_calibrationSpinBox->setValue(0);
    m_calibrationUnitsLabel->setText(xpolgui::kUndefinedUnitsLabel);
  }
}

unsigned short int pReadoutModeTab::getPixelAddressX()
{
  return m_calPixAddressXSpinBox->value();
}

void pReadoutModeTab::displayPixelAddressX(unsigned short int address)
{
  m_calPixAddressXSpinBox->setValue(address);
}

unsigned short int pReadoutModeTab::getPixelAddressY()
{
  return m_calPixAddressYSpinBox->value();
}

void pReadoutModeTab::displayPixelAddressY(unsigned short int address)
{
  m_calPixAddressYSpinBox->setValue(address);
}

void pReadoutModeTab::displayConfiguration(pDetectorConfiguration
					   *configuration,
					   int visualizationMode)
{
  displayReadoutMode(configuration->getReadoutMode());
  displayBufferMode(configuration->getBufferMode());
  displayCalibrationSignal(configuration->getCalibrationDac(),
			   visualizationMode);
  displayPixelAddressX(configuration->getPixelAddressX());
  displayPixelAddressY(configuration->getPixelAddressY());
}
