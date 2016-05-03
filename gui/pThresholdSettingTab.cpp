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

#include "pThresholdSettingTab.h"

pThresholdSettingTab::pThresholdSettingTab()
  : pQtCustomTab("Threshold settings")
{
  m_thresholdsBounded = false;
  setupThresholdWidgets();
  setupModeWidgets();
  setupReferenceWidgets();
  setupConnections();
  setupToolTips();
  freezeSize(xpolgui::kTabGroupBoxWidth);
  changeSettingMode(true);
}

void pThresholdSettingTab::setupToolTips()
{
  m_refreshReferenceButton->setToolTip("Refresh the threshold reference");
}

void pThresholdSettingTab::setupThresholdWidgets()
{
  m_groupBoxGridLayout->setSpacing(xpolgui::kTabGridLayoutSpacing);
  for (int i = 0; i < NUM_READOUT_CLUSTERS; i++){
    addThresholdWidgets(i);
  }
  disableLastThresholdWidget();
}

void pThresholdSettingTab::setupModeWidgets()
{
  int row = m_groupBoxGridLayout->rowCount();
  m_groupBoxGridLayout->setRowMinimumHeight(row, xpolgui::kTabRowsSpacing);
  row ++;
  m_modeCheckBox = new QCheckBox("All the same (linked to cluster 0)", this);
  m_groupBoxGridLayout->addWidget(m_modeCheckBox, row, 0, 1, 7);
}

void pThresholdSettingTab::setupReferenceWidgets()
{
  int row = m_groupBoxGridLayout->rowCount();
  m_groupBoxGridLayout->setRowMinimumHeight(row, xpolgui::kTabRowsSpacing);
  m_referenceLabel      = new pQtCustomTextLabel(this, "Reference");
  m_referenceDisplay    = new pQtCustomTextLabel(this, "");
  m_referenceUnitsLabel = new pQtCustomTextLabel(this,
						 xpolgui::kDACUnitsLabel);
  m_referenceDisplay->setDataStyle();
  m_referenceLabel->setLabelStyle();
  m_groupBoxGridLayout->addWidget(m_referenceLabel, row + 1, 0);
  m_groupBoxGridLayout->addWidget(m_referenceDisplay, row + 1, 1);
  m_groupBoxGridLayout->addWidget(m_referenceUnitsLabel, row + 1, 3);
  m_referenceDisplay->setFixedSize(xpolgui::kTabSpinBoxesWidth,
				   xpolgui::kTabSpinBoxesHeight);
  m_refreshReferenceButton = new pQtCustomPushButton(this, "reload");
  m_groupBoxGridLayout->addWidget(m_refreshReferenceButton, row + 1, 5);
}

void pThresholdSettingTab::addThresholdWidgets(int clusterId)
{
  int row = clusterId % (NUM_READOUT_CLUSTERS/2);
  int column = 5*(clusterId/(NUM_READOUT_CLUSTERS/2));
  m_thresholdLabel[clusterId] =
    new pQtCustomTextLabel(this, QString("Cluster %1").arg(clusterId));
  m_thresholdSpinBox[clusterId] = new QSpinBox(this);  
  m_thresholdUnitsLabel[clusterId] = new pQtCustomTextLabel(this, "        ");
  m_thresholdLabel[clusterId]->setFixedWidth(xpolgui::kTabTextLabelsWidth);
  m_thresholdLabel[clusterId]->setLabelStyle();
  m_thresholdSpinBox[clusterId]->setFixedWidth(xpolgui::kTabSpinBoxesWidth);
  m_groupBoxGridLayout->addWidget(m_thresholdLabel[clusterId], row ,column);
  m_groupBoxGridLayout->addWidget(m_thresholdSpinBox[clusterId], row,
				  column + 1);
  m_groupBoxGridLayout->addWidget(m_thresholdUnitsLabel[clusterId], row,
				  column + 3);
  m_groupBoxGridLayout->setColumnMinimumWidth(2,
					      xpolgui::kTabColumnsSmallSpacing);
  m_groupBoxGridLayout->setColumnMinimumWidth(4,
					      xpolgui::kTabColumnsMidSpacing);
  m_groupBoxGridLayout->setColumnStretch(4, 1);
  m_groupBoxGridLayout->setColumnMinimumWidth(7,
					      xpolgui::kTabColumnsSmallSpacing);
}

void pThresholdSettingTab::enableThresholdWidgets(int clusterId)
{
  m_thresholdLabel[clusterId]->setEnabled(true);
  m_thresholdSpinBox[clusterId]->setEnabled(true);
  m_thresholdUnitsLabel[clusterId]->setEnabled(true);
}

void pThresholdSettingTab::disableThresholdWidgets(int clusterId)
{
  m_thresholdLabel[clusterId]->setEnabled(false);
  m_thresholdSpinBox[clusterId]->setEnabled(false);
  m_thresholdUnitsLabel[clusterId]->setEnabled(false);
}

void pThresholdSettingTab::disableLastThresholdWidget()
{
  // This is an hack doe to the fact that the last two ADC channel are
  // effectively link to each other.
  int lastId = NUM_READOUT_CLUSTERS - 1;
  m_thresholdLabel[lastId]->setDisabled(true);
  m_thresholdSpinBox[lastId]->setDisabled(true);
  m_thresholdUnitsLabel[lastId]->setDisabled(true);
  connect(m_thresholdSpinBox[lastId - 1], SIGNAL(valueChanged(int)),
	  m_thresholdSpinBox[lastId], SLOT(setValue(int)));
}

short unsigned int pThresholdSettingTab::getThreshold (int clusterId,
						       int visualizationMode)
{
  short unsigned int threshold = m_thresholdSpinBox[clusterId]->value();
  if (visualizationMode == xpolgui::kDacVisualizationCode){
    return threshold;
  } else if (visualizationMode == xpolgui::kChargeVisualizationCode){
    return xpoldetector::thrChargeToDac(threshold);
  } else if (visualizationMode == xpolgui::kVoltageVisualizationCode){
    return xpoldetector::thrVoltageToDac(threshold);
  } else {
    *xpollog::kWarning << "Undefined visualization mode." << endline;
    return 0;
  }
}

void pThresholdSettingTab::displayThreshold(int clusterId, int threshold,
					    int visualizationMode)
{
  if (visualizationMode == xpolgui::kDacVisualizationCode){
    m_thresholdSpinBox[clusterId]->setMinimum(xpoldetector::kThresholdDacMin);
    m_thresholdSpinBox[clusterId]->setMaximum(xpoldetector::kThresholdDacMax);
    m_thresholdSpinBox[clusterId]->setValue(threshold);
    m_thresholdUnitsLabel[clusterId]->setText(xpolgui::kDACUnitsLabel);
  } else if (visualizationMode == xpolgui::kChargeVisualizationCode){
    m_thresholdSpinBox[clusterId]->
      setMinimum(xpoldetector::kThresholdChargeMin);
    m_thresholdSpinBox[clusterId]->
      setMaximum(xpoldetector::kThresholdChargeMax);
    m_thresholdSpinBox[clusterId]->
      setValue(xpoldetector::thrDacToCharge(threshold));
    m_thresholdUnitsLabel[clusterId]->setText(xpolgui::kChargeUnitsLabel);
  } else if (visualizationMode == xpolgui::kVoltageVisualizationCode){
    m_thresholdSpinBox[clusterId]->
      setMinimum(xpoldetector::kThresholdVoltageMin);
    m_thresholdSpinBox[clusterId]->
      setMaximum(xpoldetector::kThresholdVoltageMax);
    m_thresholdSpinBox[clusterId]->
      setValue(xpoldetector::thrDacToVoltage(threshold));
    m_thresholdUnitsLabel[clusterId]->setText(xpolgui::kVoltageUnitsLabel);
  } else {
    *xpollog::kWarning << "Undefined visualization mode." << endline;
    m_thresholdSpinBox[clusterId]->setValue(0);
    m_thresholdUnitsLabel[clusterId]->setText(xpolgui::kUndefinedUnitsLabel);
  }
}

short unsigned int pThresholdSettingTab::getReference()
{
  // To be implemented, if needed.
  return 0;
}

void pThresholdSettingTab::displayReference(unsigned short reference,
					    int visualizationMode)
{
  *xpollog::kDebug << "Updating reference voltage." << endline;
  if (visualizationMode == xpolgui::kDacVisualizationCode){
    m_referenceDisplay->setText(QString::number(reference));
    m_referenceUnitsLabel->setText(xpolgui::kDACUnitsLabel);
  } else if (visualizationMode == xpolgui::kChargeVisualizationCode){
    m_referenceDisplay->
      setText(QString::number(xpoldetector::refDacToCharge(reference)));
    m_referenceUnitsLabel->setText(xpolgui::kChargeUnitsLabel);
  } else if (visualizationMode == xpolgui::kVoltageVisualizationCode){
    m_referenceDisplay->
      setText(QString::number(xpoldetector::refDacToVoltage(reference)));
    m_referenceUnitsLabel->setText(xpolgui::kVoltageUnitsLabel);
  } else {
    *xpollog::kWarning << "Undefined visualization mode." << endline;
    m_referenceDisplay->setText(xpolgui::kNotAvailable);
    m_referenceUnitsLabel->setText(xpolgui::kUndefinedUnitsLabel);
  }
}

void pThresholdSettingTab::changeSettingMode(int mode)
{
  if (mode){
    m_thresholdsBounded = true;
    for (int i = 1; i < NUM_READOUT_CLUSTERS; i++){
      disableThresholdWidgets(i);
      m_thresholdSpinBox[i]->setValue(m_thresholdSpinBox[0]->value());
      connect(m_thresholdSpinBox[0], SIGNAL(valueChanged(int)),
	      m_thresholdSpinBox[i], SLOT(setValue(int)));
    }
    if (!m_modeCheckBox->isChecked()){
      m_modeCheckBox->setChecked(true);
    }
  } else {
    m_thresholdsBounded = false;
    for (int i = 1; i < NUM_READOUT_CLUSTERS; i++){
      enableThresholdWidgets(i);
      disconnect(m_thresholdSpinBox[0], SIGNAL(valueChanged(int)),
		 m_thresholdSpinBox[i], SLOT(setValue(int)));      
    }
    if (m_modeCheckBox->isChecked()){
      m_modeCheckBox->setChecked(false);
    }
  }
  disableLastThresholdWidget();
}

void pThresholdSettingTab::setupConnections()
{
  connect(m_modeCheckBox, SIGNAL(stateChanged(int)),
				 this, SLOT(changeSettingMode(int)));
}

void pThresholdSettingTab::displayConfiguration(pDetectorConfiguration
						*configuration,
						int visualizationMode)
{
  for (int i = 0; i < NUM_READOUT_CLUSTERS; i++){
    displayThreshold(i, configuration->thresholdDac(i), visualizationMode);
  }
}
