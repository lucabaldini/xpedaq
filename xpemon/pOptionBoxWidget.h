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

#include "pQtCustomTextLabel.h"
#include "pQtGroupBoxWidget.h"

class pOptionBoxWidget: public pQtGroupBoxWidget
{

  Q_OBJECT

  public:

    pOptionBoxWidget(unsigned int socketPort, double refreshInterval,
                     unsigned int  zeroSupThreshold,
                     QWidget *parent=0);
    
    void options(unsigned int &socketPort, double &refreshInterval, 
                 unsigned int  &zeroSupThreshold);
  
  public slots:

    void activateWidgets();
    void disableWidgets();
  
  private:
  
    void initalizeText();
    void readSocketPort(unsigned int &socketPort);
    void readRefreshInterval(double &refreshInterval);
    void readZeroSupThreshold(unsigned int &zeroSupThreshold);
  
    pQtCustomTextLabel *m_socketPortLabel;
    QLineEdit *m_socketPortEdit;
    pQtCustomTextLabel *m_refreshIntervalLabel;
    QLineEdit *m_refreshIntervalEdit;
    pQtCustomTextLabel *m_zeroSupThrLabel;
    QLineEdit *m_zeroSupThrEdit;    
    QString m_socketPortText;
    QString m_refreshIntervalText;
    QString m_zeroSupThresholdText;
};

#endif // OPTIONBOXWIDGET_H
