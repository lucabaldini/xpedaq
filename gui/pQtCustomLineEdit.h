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

/* Class descrybing a QTextEdit object for controlled number insertion.
   Input inserted by the user must be of the right type and comprised in a
   given range.
   Since Q_OBJECT does not mix well with template, we provide a virtual base
   class setting all the signal/slot connections and a derived template class
   for the actual implementation.
*/

#ifndef PQTCUSTOMLINEEDIT_H
#define PQTCUSTOMLINEEDIT_H

#include <QString>
#include <QLineEdit>

class pQtCustomLineEditBase : public QLineEdit
{
  Q_OBJECT
  
  public:
  
    explicit pQtCustomLineEditBase();

  signals:
    
    void inputAccepted();
  
  public slots:

    virtual void validate() = 0;
};


template <typename T>
class pQtCustomLineEdit : public pQtCustomLineEditBase
{
  public:
  
    explicit pQtCustomLineEdit(T initialVal =0);
    explicit pQtCustomLineEdit(T initialVal, T min, T max);
    T value();
    T rangeMin(){return m_min;}
    T rangeMax(){return m_max;}
    void setVal(T val);
    void setRangeMin(T min);
    void setRangeMax(T max);
    void setRange(T min, T max);
    virtual void validate();
    
  protected:
  
    T m_value;
    T m_min;
    T m_max;
};


/*A few utility functions*/
namespace convertions{
  bool qStrToNumber(const QString& str, double &result);
  bool qStrToNumber(const QString& str, int &result);
  bool qStrToNumber(const QString& str, unsigned int &result);
}

#endif //PQTCUSTOMLINEEDIT_H
