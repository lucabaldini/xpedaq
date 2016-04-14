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

#ifndef PTHRESHOLDSETTINGTAB_H
#define PTHRESHOLDSETTINGTAB_H

#include <iostream>
#include <math.h>

#include <QWidget>
#include <QString>
#include <QSpinBox>
#include <QCheckBox>

#include "pQtCustomTab.h"
#include "pQtCustomTextLabel.h"
#include "pQtCustomPushButton.h"
#include "pDetectorConfiguration.h"

#include "xpollog.h"

class pThresholdSettingTab : public pQtCustomTab
{

  Q_OBJECT

 public:
  pThresholdSettingTab();
  ~pThresholdSettingTab() {;}

  short unsigned int getThreshold (int clusterId, int visualizationMode);
  void displayThreshold(int clusterId, int threshold, int visualizationMode);
  short unsigned int getReference();
  void displayConfiguration(pDetectorConfiguration *configuration,
			    int visualizationMode);
  pQtCustomPushButton *getRefreshRefButton(){return m_refreshReferenceButton;}
  
 public slots:
  void displayReference(unsigned short reference, int visualizationMode);

 private slots:
  void changeSettingMode(int mode);

 private:
  pQtCustomTextLabel *m_thresholdLabel[NUM_READOUT_CLUSTERS];
  QSpinBox *m_thresholdSpinBox[NUM_READOUT_CLUSTERS];
  pQtCustomTextLabel *m_thresholdUnitsLabel[NUM_READOUT_CLUSTERS];
  QCheckBox *m_modeCheckBox;
  bool m_thresholdsBounded;
  pQtCustomTextLabel *m_referenceLabel;
  pQtCustomTextLabel *m_referenceDisplay;
  pQtCustomTextLabel *m_referenceUnitsLabel;
  pQtCustomPushButton *m_refreshReferenceButton;
  void setupToolTips();
  void addThresholdWidgets(int clusterId);
  void enableThresholdWidgets(int clusterId);
  void disableThresholdWidgets(int clusterId);
  void disableLastThresholdWidget();
  void setupThresholdWidgets();
  void setupModeWidgets();
  void setupReferenceWidgets();
  void setupConnections();
};

#endif
