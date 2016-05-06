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

  // Access functions.
  unsigned short clockFrequency() const;
  unsigned short clockShift() const;
  unsigned short numPedSamples() const;
  unsigned short pedSubDelay() const;
  unsigned short trgEnableDelay() const;
  unsigned short minWindowSize() const;
  unsigned short maxWindowSize() const;

  // Display functions.
  void displayClockFrequency(unsigned short frequency);
  void displayClockShift(unsigned short shift);
  void displayNumPedsSamples(unsigned short samples);
  void displayPedSubDelay(unsigned short delay);
  void displayTrgEnableDelay(unsigned short delay);
  void displayMinWindowSize(unsigned short size);
  void displayMaxWindowSize(unsigned short size);
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
