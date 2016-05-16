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


#include "pOption.h"


pOption::pOption(std::string longopt, char shortopt, pVariant value,
		 std::string help, bool required, bool initialized) :
  m_longOpt(longopt),
  m_shortOpt(shortopt),
  m_value(value),
  m_help(help),
  m_required(required),
  m_initialized(initialized)
{}


std::ostream& pOption::fillStream(std::ostream& os) const
{
  os << "-" << m_shortOpt << ", --" << std::setw(15) << std::left
     << m_longOpt;
  if (type() != pVariant::Boolean) {
    os << std::setw(7) << typeString();
  } else {
    os << std::setw(7) << "";
  }
  os << std::right << m_help;
  if (type() != pVariant::Boolean) {
    if (m_initialized) os << " [" << m_value << "]";
    else os << " [none]";
  }
  if (m_required) os << " *";
  return os;
}
