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

#include <iostream>
#include <limits>

#include "pQtCustomLineEdit.h"


pQtCustomLineEditBase::pQtCustomLineEditBase() :
  QLineEdit()
{
  setFixedWidth(80);
  setFixedHeight(21);
  connect (this, SIGNAL(editingFinished()),
           this, SLOT(validate()));
}


/****************************************/


template <typename T>
pQtCustomLineEdit<T>::pQtCustomLineEdit(T initialVal) : 
  pQtCustomLineEditBase()
{
  m_min = std::numeric_limits<T>::lowest(); //NOTE: min() is the positive min!
  m_max = std::numeric_limits<T>::max();
  setVal(initialVal);
}


template <typename T>
pQtCustomLineEdit<T>::pQtCustomLineEdit(T initialVal, T min, T max) : 
  pQtCustomLineEditBase(),
  m_min(min),
  m_max(max)
{
  setVal(initialVal);
}


template <typename T>
T pQtCustomLineEdit<T>::value()
{
  return m_value;
}


template <typename T>
void pQtCustomLineEdit<T>::setVal(T val)
{
  //must be comprised between min and max
  if (val < m_min || val > m_max){
    std::cout << "Value inserted out of range ( " << val
              << "). Input has been ignored." << std::endl;
  }
  else{
    m_value = val;
  }
  setText(QString::number(m_value));
}


template <typename T>
void pQtCustomLineEdit<T>::setRangeMin(T min)
{
  //min must be lower than max
  if (min >= m_max){
    std::cout << "Lower limit inserted is not smaller than upper limit. Input"
              << " has been ignored." << std::endl;
  }
  else {
    m_min = min;
  }
}


template <typename T>
void pQtCustomLineEdit<T>::setRangeMax(T max)
{
  //max must be grater than min
  if (max <= m_min){
    std::cout << "Upper limit inserted is not greater than lower limit. Input"
              << " has been ignored." << std::endl;
  }
  else {
    m_max = max;
  }
}


template <typename T>
void pQtCustomLineEdit<T>::validate()
{
  T newVal;
  bool convSuccess = convertions::qStrToNumber(text(), newVal);
  // if the value inserted is invalid restore the stored value
  if (!convSuccess || newVal > m_max || newVal < m_min){
    std::cout << "Invalid input " << text().toStdString() << std::endl;
    setText(QString::number(m_value));
    return;
  }
  // else override the stored value wth the inserted value
  m_value = newVal;
  emit inputAccepted();
}


/**********************************/


bool convertions::qStrToNumber(const QString& str, double& result)
{
  bool success;
  double tmpResult;
  tmpResult = str.toDouble(&success);
  if (success)
    result = tmpResult;
  return success;
}


bool convertions::qStrToNumber(const QString& str, int& result)
{
  bool success;
  int tmpResult;
  tmpResult = str.toInt(&success);
  if (success)
    result = tmpResult;
  return success;
}


bool convertions::qStrToNumber(const QString& str, unsigned int& result)
{
  bool success;
  unsigned int tmpResult;
  tmpResult = str.toUInt(&success);
  if (success)
    result = tmpResult;
  return success;
}


// We declare the template class for a set of types (won't work for others)
// Mind that these lines *have to* be at the end of the file for the whole
// thing to work, see:
// http://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor
template class pQtCustomLineEdit<double>;
template class pQtCustomLineEdit<int>;
template class pQtCustomLineEdit<unsigned int>;
