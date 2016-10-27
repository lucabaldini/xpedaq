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

#ifndef PCUTBOXWIDGET_H
#define PCUTBOXWIDGET_H


#include "pMonitorPreferences.h"
#include "pQtCustomTextLabel.h"
#include "pQtCustomLineEdit.h"
#include "pQtGroupBoxWidget.h"
#include "pMinMaxOptionPair.h"


class pCutBoxWidget: public pQtGroupBoxWidget
{

  Q_OBJECT

  public:

    explicit pCutBoxWidget(const pMonitorPreferences& preferences,
			   QWidget *parent = 0);
    void options(pMonitorPreferences* preferences);
  
  public slots:

    void enable();
    void disable();
  
  private:
    
    void readElongationLimits();
    void readClusterSizeLimits();
    void readPulseHeightLimits();
    void readWindowSizeLimits();
  
    pQtCustomTextLabel *m_elongationLabel;
    pMinMaxOptionPair<double> *m_elongationLimits;    
    pQtCustomTextLabel *m_clusterSizeLabel;
    pMinMaxOptionPair<int> *m_clusterSizeLimits;    
    pQtCustomTextLabel *m_pulseHeightLabel;
    pMinMaxOptionPair<int> *m_pulseHeightLimits;
    pQtCustomTextLabel *m_windowSizeLabel;
    pMinMaxOptionPair<int> *m_windowSizeLimits;
    pMonitorPreferences m_preferences;
};

#endif // PCUTBOXWIDGET_H
