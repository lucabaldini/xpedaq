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

#ifndef PADVANCEDSETTINGSTAB_H
#define PADVANCEDSETTINGSTAB_H

#include <iostream>
#include <QString>
#include <QSpinBox>
#include <QComboBox>
#include <QStringList>
#include "pQtCustomTextLabel.h"
#include "pQtCustomTab.h"
#include "pDetectorConfiguration.h"
#include "xpolgui.h"
#include "xpoldetector.h"

class pAdvancedSettingsTab : public pQtCustomTab
{

  Q_OBJECT

 public:

  pAdvancedSettingsTab();
  ~pAdvancedSettingsTab() {;}
  unsigned short int getTimingCode();
  void displayTimingCode(unsigned short int code);
  unsigned short int getNumPedSamples();
  void displayNumPedsSamples(unsigned short int samples);
  unsigned short int getPedSubDelay();
  void displayPedSubDelay(unsigned short int delay);
  unsigned short int getTrgEnableDelay();
  void displayTrgEnableDelay(unsigned short int delay);
  unsigned short int getMinWindowSize();
  void displayMinWindowSize(unsigned short int size);
  unsigned short int getMaxWindowSize();
  void displayMaxWindowSize(unsigned short int size);
  void displayConfiguration(pDetectorConfiguration *configuration);

 private:
  
  QString m_clockFasterLabel;
  QString m_clockFastLabel;
  QString m_clockSlowLabel;
  QString m_clockSlowerLabel;
  QString m_subSamplesZeroLabel;
  QString m_subSamplesSmallerLabel;
  QString m_subSamplesSmallLabel;
  QString m_subSamplesLargeLabel;
  QString m_subSamplesLargerLabel;
  pQtCustomTextLabel *m_clockFrequencyLabel;
  QComboBox *m_clockFrequencyComboBox;
  pQtCustomTextLabel *m_clockFrequencyUnitsLabel;
  pQtCustomTextLabel *m_clockShiftLabel;
  QSpinBox *m_clockShiftSpinBox;
  pQtCustomTextLabel *m_clockShiftUnitsLabel;
  pQtCustomTextLabel *m_numPedSamplesLabel;
  QComboBox *m_numPedSamplesComboBox;
  pQtCustomTextLabel *m_pedSubDelayLabel;
  QSpinBox *m_pedSubDelaySpinBox;
  pQtCustomTextLabel *m_pedSubDelayUnitsLabel;
  pQtCustomTextLabel *m_trgEnableDelayLabel;
  QSpinBox *m_trgEnableDelaySpinBox;
  pQtCustomTextLabel *m_trgEnableDelayUnitsLabel;
  pQtCustomTextLabel *m_minWindowSizeLabel;
  QSpinBox *m_minWindowSizeSpinBox;
  pQtCustomTextLabel *m_maxWindowSizeLabel;
  QSpinBox *m_maxWindowSizeSpinBox;
  void setupTimingWidgets();
  void setupPedSubWidgets();
  void setupWindowWidgets();
};

#endif
