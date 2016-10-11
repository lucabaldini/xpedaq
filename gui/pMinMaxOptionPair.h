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

/* Class descrybing a pair of QTextEdit objects used to set a min-max
   range for some option parameter. The value inserted by the user are
   checked, so that min is always smaller than max. Furthermore, a bottom
   value and a top value can be setted to constrain the input parameters,
   so that bottom <= min < max <= top.
   Since Q_OBJECT does not mix well with template, we provide a virtual base
   class setting all the signal/slot connections and a derived template class
   for the actual implementation.
*/

#ifndef PMINMAXOPTIONPAIR_H
#define PMINMAXOPTIONPAIR_H

#include <iostream>

#include <QString>
#include <QLineEdit>
#include <QWidget>

#include "pQtGridLayoutWidget.h"
#include "pQtCustomTextLabel.h"
#include "pQtCustomLineEdit.h"


class pMinMaxOptionPairBase : public pQtGridLayoutWidget
{
  
  Q_OBJECT
  
  public:
  
    pMinMaxOptionPairBase(QWidget *parent) : pQtGridLayoutWidget(parent) {;}
    
  public slots:
  
    virtual void updateMinRange() = 0;
    virtual void updateMaxRange() = 0;
    virtual void setDisabled(bool disabled) = 0;
  
};


template <typename T>
class pMinMaxOptionPair : public pMinMaxOptionPairBase
{
  public:
  
    pMinMaxOptionPair(QWidget *parent, QString labelName, T min, T max);   
    void readMin(T& min);
    void readMax(T& max);
    void readOptions(T& min, T& max);
    virtual void updateMinRange();
    virtual void updateMaxRange();
    virtual void setDisabled(bool disabled);
    void setBottom(T bottom);
    void setTop(T top);
    
  protected:
  
    bool inputValid(T input);
    
    pQtCustomTextLabel *m_label;
    pQtCustomLineEdit<T> *m_minEdit;
    pQtCustomLineEdit<T> *m_maxEdit;
    T m_min;
    T m_max;
};

#endif //PMINMAXOPTIONPAIR_H
