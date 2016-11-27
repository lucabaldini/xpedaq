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

#ifndef XPEREGREGISTERTAB_H
#define XPEREGREGISTERTAB_H

#include <QWidget>
#include <QString>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>

#include "pQtCustomTab.h"
#include "pQtCustomTextLabel.h"
#include "xpoldetector.h"
#include "xperegUserPreferences.h"


class xperegRegisterTab : public pQtCustomTab
{

  Q_OBJECT

  public:
   
    xperegRegisterTab();
    void displayUserPreferences(xperegUserPreferences *preferences);
    void userPreferences(xperegUserPreferences &preferences);

    // Access methods.
    unsigned short pixelAddressX() const;
    unsigned short pixelAddressY() const;
    unsigned short configuration() const;
    bool randomShuffle() const;
    int readoutRepeat() const;
    int readoutInterval() const;
  
  public slots:

    
  private slots:


  private:
   
    pQtCustomTextLabel *m_xLabel;
    QSpinBox *m_xSpinBox;
    QLineEdit *m_xDisplay;
    pQtCustomTextLabel *m_yLabel;
    QSpinBox *m_ySpinBox;
    QLineEdit *m_yDisplay;
    pQtCustomTextLabel *m_configLabel;
    QSpinBox *m_configSpinBox;
    QLineEdit *m_configDisplay;
    QCheckBox *m_randomCheckBox;
    pQtCustomTextLabel *m_readoutRepeatLabel;
    QSpinBox *m_readoutRepeatSpinBox;
    pQtCustomTextLabel *m_readoutIntervalLabel;
    QSpinBox *m_readoutIntervalSpinBox;
    void setupWidgets();
    void setupConnections();
};

#endif //XPEREGREGISTERTAB_H
