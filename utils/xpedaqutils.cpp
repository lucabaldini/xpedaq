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

#include "xpedaqutils.h"


void xpedaqutils::startmsg()
{
  std::cout << "\n    Welcome to xpedaq version " << __XPEDAQ_VERSION__
	    << " (built on " << __XPEDAQ_BUILD_DATE__ << ").\n"
            << "\n    Copyright (C) 2007--2016 the xpedaq team\n\n"
            << "    xpedaq comes with ABSOLUTELY NO WARRANTY.\n"
            << "    This is free software, and you are welcome to redistribute "
            << "it under certain\n"
            << "    conditions. See the LICENSE file for details.\n\n"
            << "    Visit https://github.com/lucabaldini/xpedaq for "
            << "more information.\n\n";
}


std::string xpedaqutils::hline(bool eol)
{
  std::stringstream s("");
  for (unsigned int i = 0; i < xpedaqutils::kTerminalWidth; i++) {
    s << xpedaqutils::kHorLineChar;
  }
  if (eol) s << xpedaqutils::kEolChar;
  return s.str();
}


std::string xpedaqutils::title(std::string text, bool eol)
{
  std::stringstream s("");
  const unsigned int wl = (xpedaqutils::kTerminalWidth -
                           text.size() -
                           2*xpedaqutils::kTitlePadding)/2;
  const unsigned int wr = xpedaqutils::kTerminalWidth - 
    text.size() - 2*xpedaqutils::kTitlePadding - wl;
  for (unsigned int i = 0; i < wl; i++) {
    s << xpedaqutils::kHorLineChar;
  }
  for (unsigned int i = 0; i < xpedaqutils::kTitlePadding; i++) {
    s << " ";
  }
  s << text;
  for (unsigned int i = 0; i < xpedaqutils::kTitlePadding; i++) {
    s << " ";
  }
  for (unsigned int i = 0; i < wr; i++) {
    s << xpedaqutils::kHorLineChar;
  }
  if (eol) s << xpedaqutils::kEolChar;
  return s.str();
}
