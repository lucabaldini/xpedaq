/***********************************************************************
Copyright (C) 2007, 2008 by Luca Baldini (luca.baldini@pi.infn.it),
Johan Bregeon, Massimo Minuti and Gloria Spandre.

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

#ifndef PQCUSTOMTEXTLABEL_H
#define PQCUSTOMTEXTLABEL_H

#include <iostream>

#include <QString>
#include <QLabel>
#include <QFrame>
#include <QPalette>

/*! \brief A custom text label.

It is essentially a QLabel with some overrided methods for convenience.*/


class pQtCustomTextLabel : public QLabel
{

 public:
  
  /*! \brief Basic constructor.*/
  pQtCustomTextLabel(QWidget *parent, QString text="");
  /*! \brief Do nothing destructor.*/
  ~pQtCustomTextLabel()
    {;}
  /*! \brief Set the appearance of the widget to display a field label.*/
  void setLabelStyle(int width = -1);
  /*! \brief Set the appearance of the widget to display a field value.*/
  void setDataStyle();
  /*! \brief Set the widget text.*/
  void setText(QString value)
    {QLabel::setText(value);}
  /*! \brief Set the widget text.*/
  void setText(std::string value)
    {QLabel::setText(QString::fromStdString(value));}
  /*! \brief Set the widget text.*/
  void setText(char* value)
    {QLabel::setText(QString("%1").arg(value));}
  /*! \brief Set the widget text.*/
  void setText(int value)
    {QLabel::setText(QString("%1").arg(value));}
  /*! \brief Set the widget text.*/
  void setText(unsigned int value)
    {QLabel::setText(QString("%1").arg(value));}
  /*! \brief Set the widget text.*/
  void setText(long int value)
    {QLabel::setText(QString("%1").arg(value));}
  /*! \brief Set the widget text.*/
  void setText(double value)
    {QLabel::setText(QString("%1").arg(value, 1, 'f', 2));}

 protected:
  /*! \brief Make the widget frame visible.*/
  void setFrameVisible();
  /*! \brief Make the background color white.*/
  void setBrightBackground();

 private:

};

#endif
