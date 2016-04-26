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

#ifndef XPEDAQUTILS_H
#define XPEDAQUTILS_H

#include <iostream>
#include <string>
#include <sstream>

#include "__version__.h"


namespace xpedaqutils{

  static const unsigned short int kTerminalWidth = 80;
  static const unsigned short int kTitlePadding = 1;
  static const std::string kHorLineChar = "-";
  static const std::string kEolChar = "\n";

  void startmsg();
  std::string hline(bool eol = false);
  std::string title(std::string text, bool eol = false);
  template <class T> std::string keyval(std::string key, T value,
					std::string units = "",
					unsigned int width = 25,
					bool eol = true)
    {
      std::stringstream s("");
      s << key;
      for (unsigned int i = 0; i < width - key.size(); i++) {
	s << ".";
      }
      s << ": " << value;
      if (units.size()) s << " " << units;
      if (eol) s << kEolChar;
      return s.str();
    }
}

#endif //XPEDAQUTILS_H

