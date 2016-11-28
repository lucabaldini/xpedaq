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


#include "pChrono.h"


pChrono::pChrono()
{
  start();
}


double pChrono::now() const
{
  auto tp = std::chrono::system_clock::now();
  auto us = std::chrono::time_point_cast<std::chrono::microseconds>(tp);
  return 1.0e-6*us.time_since_epoch().count();
}


double pChrono::start()
{
  m_start = now();
  return m_start;
}


double pChrono::stop()
{
  m_stop = now();
  return m_stop;
}


double pChrono::split() const
{
  return now() - m_start;
}


std::string pChrono::double2datetime(double t) const
{
  // Convert the seconds to date and time.
  long int seconds = static_cast<long int> (t);
  // Create the datetime string.
  std::string datetime(ctime(&seconds));
  // Don't forget stripping the newline.
  datetime.erase(datetime.size() - 1);
  return datetime;
}


std::ostream& pChrono::fillStream(std::ostream& os) const
{  
  os << std::fixed << now() << " s";
  return os;
}
