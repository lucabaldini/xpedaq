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

#ifndef OPTIONBOXWIDGET_H
#define OPTIONBOXWIDGET_H

#include <QLineEdit>
#include <QGridLayout>
#include <QWidget>
#include <QString>
#include <QCheckBox>

#include "pMonitorPreferences.h"
#include "pQtCustomTextLabel.h"
#include "pQtCustomLineEdit.h"
#include "pQtGroupBoxWidget.h"
#include "pMinMaxOptionPair.h"

class pOptionBoxWidget: public pQtGroupBoxWidget
{

  Q_OBJECT

  public:

    explicit pOptionBoxWidget(const pMonitorPreferences& preferences,
                              QWidget *parent = 0);
    
    void options(pMonitorPreferences* preferences);
  
  signals:
  
    void drawReconInfoCheckBoxStatusChanged(int state);
  
  public slots:

    void activateWidgets();
    void disableWidgets();
    void updateReconInfoBoxStatus(int state);
  
  private:
  
    void readSocketPort();
    void readRefreshInterval();
    void readZeroSupThreshold();
    void readElongationLimits();
    void readClusterSizeLimits();
    void readPulseHeightLimits();
  
    pQtCustomTextLabel *m_socketPortLabel;
    pQtCustomLineEdit<unsigned int> *m_socketPortEdit;
    pQtCustomTextLabel *m_refreshIntervalLabel;
    pQtCustomLineEdit<double> *m_refreshIntervalEdit;
    pQtCustomTextLabel *m_zeroSupThrLabel;
    pQtCustomLineEdit<unsigned int> *m_zeroSupThrEdit;
    pMinMaxOptionPair<double> *m_elongationLimits;    
    pMinMaxOptionPair<int> *m_clusterSizeLimits;    
    pMinMaxOptionPair<int> *m_pulseHeightLimits;
    QCheckBox *m_drawReconInfoCheckBox;
    pMonitorPreferences m_preferences;
};

#endif // OPTIONBOXWIDGET_H
