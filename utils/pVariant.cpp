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


#include "pVariant.h"


std::string pVariant::typeString() const
{
  switch (m_type) {
  case Boolean  : return "bool";
  case Integer  : return "int";
  case Floating : return "double";
  case Literal  : return "string";
  default       : return "";
  }
}


std::ostream& pVariant::fillStream(std::ostream& os) const
{
  switch (m_type) {
  case Boolean : os << m_data.b; break;
  case Integer : os << m_data.i; break;
  case Floating: os << m_data.d; break;
  case Literal : os << m_string; break;
  default      : break; 
  };
  return os;
}
