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

#ifndef PQTCUSTOMTAB_H
#define PQTCUSTOMTAB_H

#include <iostream>

#include <QWidget>
#include <QString>

#include "pQtGroupBoxWidget.h"
#include "xpolgui.h"


class pQtCustomTab : public pQtGroupBoxWidget
{

 public:

  pQtCustomTab(std::string title);
  ~pQtCustomTab() {;}
  void addVerticalSpacer(int units = 1);
};

#endif
