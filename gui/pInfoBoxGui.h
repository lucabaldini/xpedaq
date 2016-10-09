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

#ifndef PINFOBOXGUI_H
#define PINFOBOXGUI_H

#include <vector>
#include <iostream>
#include <typeinfo> 

#include <QString>

#include "pQtGroupBoxWidget.h"
#include "pQtCustomTextLabel.h"
#include "xpolgui.h"


struct varField{
  pQtCustomTextLabel* nameLabel;
  pQtCustomTextLabel* dataLabel;
  
  template <typename T>
  void setField(T value){
    dataLabel->setText(value);
  }
};


class pInfoBoxGui : public pQtGroupBoxWidget
{
  public:
    explicit pInfoBoxGui(QWidget *parent);
    
    template<typename T>
    void setField(QString fieldName, T value);   
      
  protected:
    pQtCustomTextLabel* addField(QString fieldName);
    std::vector<varField> m_fields;
};


// Definition here because of template restricition
template <typename T>
void pInfoBoxGui::setField(QString fieldName, T value)
{
  for (auto &&field : m_fields){
    if (field.nameLabel->text() == fieldName){
      field.dataLabel->setText(value);
    }
  }
}

#endif // PINFOBOXGUI_H
