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


pOption::pOption(std::string longName, char shortName, pVariant value,
		 std::string help) :
  m_longName(longName),
  m_shortName(shortName),
  m_value(value),
  m_help(help),
  m_set(false)
{}


std::ostream& pOption::fillStream(std::ostream& os) const
{
  os << "-" << m_shortName << ", --" << std::setw(15) << std::left
     << m_longName;
  if (type() != pVariant::Boolean) {
    os << std::setw(7) << typeString();
  } else {
    os << std::setw(7) << "";
  }
  os << std::right << m_help;
  if (type() != pVariant::Boolean) {
    if (m_set) {
      os << " [" << m_value << "]";
    }
  }
  return os;
}
