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

#include "pMinMaxOptionPair.h"


template <typename T>
pMinMaxOptionPair<T>::pMinMaxOptionPair(QWidget *parent, T min, T max) : 
  pMinMaxOptionPairBase(parent)
{
  m_minEdit = new pQtCustomLineEdit<T>(min);
  m_maxEdit = new pQtCustomLineEdit<T>(max);
  addWidget(m_minEdit, 0, 0);
  addWidget(m_maxEdit, 0, 1);
  
  connect (m_minEdit, SIGNAL(inputAccepted()),
           this, SLOT(updateMaxRange()));
  connect (m_maxEdit, SIGNAL(inputAccepted()),
           this, SLOT(updateMinRange()));
  
  m_minEdit->setRangeMax(max);
  m_maxEdit->setRangeMin(min);
}


template <typename T>
void pMinMaxOptionPair<T>::readMin(T& min)
{
  min = m_minEdit->value();
}


template <typename T>
void pMinMaxOptionPair<T>::readMax(T& max)
{
  max = m_maxEdit->value();
}


template <typename T>
void pMinMaxOptionPair<T>::readOptions(T& min, T& max)
{
  readMin(min);
  readMax(max);
}


template <typename T>
void pMinMaxOptionPair<T>::updateMinRange()
{
  m_minEdit->setRangeMax(m_maxEdit->value());
}


template <typename T>
void pMinMaxOptionPair<T>::updateMaxRange()
{
  m_maxEdit->setRangeMin(m_minEdit->value());
}


template <typename T>
void pMinMaxOptionPair<T>::setDisabled(bool disabled)
{
  m_minEdit->setDisabled(disabled);
  m_maxEdit->setDisabled(disabled);
}


template <typename T>
void pMinMaxOptionPair<T>::setBottom(T bottom)
{
  //bottom must be smaller than top (aka maxRange of maxEdit)
  if (bottom >= m_maxEdit->rangeMax()){
    std::cout << "Lower limit inserted is not smaller than upper limit. Input"
              << " has been ignored" << std::endl;
  }
  else {
    m_minEdit->setRangeMin(bottom);
  }
};


template <typename T>
void pMinMaxOptionPair<T>::setTop(T top)
{
  //top must be grater than bottom (aka minRange of minEdit)
  if (top <= m_minEdit->rangeMin()){
    std::cout << "Upper limit inserted is not greater than lower limit. Input"
              << " has been ignored" << std::endl;
  }
  else {
    m_maxEdit->setRangeMax(top);
  }
}


// We declare the template class for a set of types (won't work for others)
// Mind that these lines *have to* be at the end of the file for the whole
// thing to work, see:
// http://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor
template class pMinMaxOptionPair<double>;
template class pMinMaxOptionPair<int>;
template class pMinMaxOptionPair<unsigned int>;
