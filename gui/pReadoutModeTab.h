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

#ifndef PREADOUTMODETAB_H
#define PREADOUTMODETAB_H

#include <iostream>
#include <math.h>
#include <QWidget>
#include <QString>
#include <QSpinBox>
#include <QComboBox>
#include <QRadioButton>
#include "pQtCustomTextLabel.h"
#include "pQtCustomTab.h"
#include "pDetectorConfiguration.h"
#include "xpoldetector.h"

class pReadoutModeTab : public pQtCustomTab
{
  
  Q_OBJECT

 public:
  pReadoutModeTab();
  ~pReadoutModeTab() {;}
  unsigned short int getReadoutMode();
  void displayReadoutMode(unsigned short int mode);
  unsigned short int getBufferMode();
  void displayBufferMode(unsigned short int mode);
  unsigned short int getCalibrationSignal(int visualizationMode);
  void displayCalibrationSignal(unsigned short int calSignal,
			     int visualizationMode);
  unsigned short int getPixelAddressX();
  void displayPixelAddressX(unsigned short int address);
  unsigned short int getPixelAddressY();
  void displayPixelAddressY(unsigned short int address);
  void displayConfiguration(pDetectorConfiguration *configuration,
			    int visualizationMode);

 public slots:
  void enableCalibrationWidgets(bool enable = true);
  void disableCalibrationWidgets(bool disable = true);
  void enableBufferModeWidgets(bool enable = true);
  void disableBufferModeWidgets(bool disable = true);

 protected:

 private:
  QString m_smallBufferLabel;
  QString m_largeBufferLabel;
  QRadioButton *m_fullFrameRadioButton;
  QRadioButton *m_windowRadioButton;
  QRadioButton *m_chargeInjectionRadioButton;
  pQtCustomTextLabel *m_bufferModeLabel;
  QComboBox *m_bufferModeComboBox;
  pQtCustomTextLabel *m_calibrationLabel;
  QSpinBox *m_calibrationSpinBox;
  pQtCustomTextLabel *m_calibrationUnitsLabel;
  pQtCustomTextLabel *m_calPixAddressXLabel;
  QSpinBox *m_calPixAddressXSpinBox;
  pQtCustomTextLabel *m_calPixAddressYLabel;
  QSpinBox *m_calPixAddressYSpinBox;
  void setupReadoutModeWidgets();
  void setupBufferModeWidgets();
  void setupCalibrationWidgets();
  void setupConnections();
};

#endif
