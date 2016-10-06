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

#ifndef INFOBOXWIDGET_H
#define INFOBOXWIDGET_H

#include <iostream>

#include <QLineEdit>
#include <QGridLayout>
#include <QWidget>

#include "pQtCustomTextLabel.h"
#include "pQtGroupBoxWidget.h"

class pInfoBoxWidget: public pQtGroupBoxWidget
{

  Q_OBJECT

  public:

    explicit pInfoBoxWidget(QWidget *parent=0);
  
  public slots:
  
    void updateCounter();
    void updateWindowSize(unsigned int colMin, unsigned int colMax,
                          unsigned int rowMin, unsigned int rowMax);
    void updateMaxCoordinates(double x, double y);
    void updateBarycenterCoordinates(double x, double y);
    void reset();
 
  private:
  
    void initalizeText();
  
    pQtCustomTextLabel *m_nEventsLabel;
    QLineEdit *m_nEventsEdit;
    pQtCustomTextLabel *m_windowSizeLabel;
    QLineEdit *m_windowSizeEdit;
    pQtCustomTextLabel *m_highestPixelLabel;
    QLineEdit *m_highestXEdit;
    QLineEdit *m_highestYEdit;
    pQtCustomTextLabel *m_barycenterLabel;
    QLineEdit *m_barycenterXEdit;
    QLineEdit *m_barycenterYEdit;
    unsigned int m_evtCounter;
};

#endif // INFBOXWIDGET_H
