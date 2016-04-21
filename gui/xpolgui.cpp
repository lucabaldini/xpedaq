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

#include "xpolgui.h"

const short unsigned int xpolgui::kUndefinedVisualizationCode = 0x0;
const short unsigned int xpolgui::kDacVisualizationCode = 0x1;
const short unsigned int xpolgui::kChargeVisualizationCode = 0x2;
const short unsigned int xpolgui::kVoltageVisualizationCode = 0x4;
const QString xpolgui::kNotAvailable("N/A");
const int xpolgui::kDaqDisplayLabelsWidth = 250;
const int xpolgui::kButtonsWidth = 40;
const int xpolgui::kButtonsHeight = 40;
const int xpolgui::kButtonsIconsSize = 18;
const int xpolgui::kButtonsSmallSpacing = 2;
const int xpolgui::kButtonsLargeSpacing = 15;
const int xpolgui::kTabGroupBoxWidth = 525;
const int xpolgui::kTabGridLayoutSpacing = 0;
const int xpolgui::kTabColumnsSmallSpacing = 8;
const int xpolgui::kTabColumnsMidSpacing = 25;
const int xpolgui::kTabRowsSpacing = 16;
const int xpolgui::kTabTextLabelsWidth = 72;
const int xpolgui::kTabTextLabelsHeight = 21;
const int xpolgui::kTabSpinBoxesWidth = 105;
const int xpolgui::kTabSpinBoxesHeight = 21;
const int xpolgui::kTextEditsHeight = 25;
const QString xpolgui::kDACUnitsLabel       = "[DAC]";
const QString xpolgui::kChargeUnitsLabel    = "[e-]";
const QString xpolgui::kVoltageUnitsLabel   = "[mV]";
const QString xpolgui::kUndefinedUnitsLabel = "[--]";
const QString xpolgui::kMHzUnitsLabel       = "[MHz]";
const QString xpolgui::knsUnitsLabel        = "[ns]";
const QString xpolgui::kusUnitsLabel        = "[us]";
