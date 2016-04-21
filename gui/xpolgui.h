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

#ifndef XPOLGUI_H
#define XPOLGUI_H

#include <QString>


namespace xpolgui{
  extern const short unsigned int kUndefinedVisualizationCode;
  extern const short unsigned int kDacVisualizationCode;
  extern const short unsigned int kChargeVisualizationCode;
  extern const short unsigned int kVoltageVisualizationCode;
  extern const QString kNotAvailable;
  extern const int kDaqDisplayLabelsWidth;
  extern const int kButtonsWidth;
  extern const int kButtonsHeight;
  extern const int kButtonsIconsSize;
  extern const int kButtonsSmallSpacing;
  extern const int kButtonsLargeSpacing;
  extern const int kTabGroupBoxWidth;
  extern const int kTabGridLayoutSpacing;
  extern const int kTabColumnsSmallSpacing;
  extern const int kTabColumnsMidSpacing;
  extern const int kTabRowsSpacing;
  extern const int kTabTextLabelsWidth;
  extern const int kTabTextLabelsHeight;
  extern const int kTabSpinBoxesWidth;
  extern const int kTabSpinBoxesHeight;
  extern const int kTextEditsHeight;
  extern const QString kDACUnitsLabel;
  extern const QString kChargeUnitsLabel;
  extern const QString kVoltageUnitsLabel;
  extern const QString kUndefinedUnitsLabel;
  extern const QString kMHzUnitsLabel;
  extern const QString knsUnitsLabel;
  extern const QString kusUnitsLabel;
}

#endif
