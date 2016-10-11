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

// We declare the template class for a set of types (won't work for others)
template class pMinMaxOptionPair<double>;
template class pMinMaxOptionPair<int>;
template class pMinMaxOptionPair<unsigned int>;


template <typename T>
pMinMaxOptionPair<T>::pMinMaxOptionPair(QWidget *parent, QString labelName,
                                     T min, T max) : 
                                     pMinMaxOptionPairBase(parent)
{
  m_label = new pQtCustomTextLabel(this, labelName);
  m_minEdit = new pQtCustomLineEdit<T>(this, min);
  m_maxEdit = new pQtCustomLineEdit<T>(this, max);
  addWidget(m_label, 0, 0);
  addWidget(m_minEdit, 0, 1);
  addWidget(m_maxEdit, 0, 2);
  
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
