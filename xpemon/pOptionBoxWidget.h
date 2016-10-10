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
#include "pQtGroupBoxWidget.h"

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
  
    void initalizeText();
    void readSocketPort();
    void readRefreshInterval();
    void readZeroSupThreshold();
    void readMinElongation();    
    void readMaxElongation();
  
    pQtCustomTextLabel *m_socketPortLabel;
    QLineEdit *m_socketPortEdit;
    pQtCustomTextLabel *m_refreshIntervalLabel;
    QLineEdit *m_refreshIntervalEdit;
    pQtCustomTextLabel *m_zeroSupThrLabel;
    QLineEdit *m_zeroSupThrEdit;
    pQtCustomTextLabel *m_minElongationLabel;
    QLineEdit *m_minElongationEdit;
    pQtCustomTextLabel *m_maxElongationLabel;    
    QLineEdit *m_maxElongationEdit;
    QCheckBox *m_drawReconInfoCheckBox;
    pMonitorPreferences m_preferences;
};

#endif // OPTIONBOXWIDGET_H
