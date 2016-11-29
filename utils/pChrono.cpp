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


/*! Do-nothing constructor.
 */
pChrono::pChrono()
{}


/*! Return the current seconds from January 1, 2016 as a double-precision
  floating point number with a (theoretical) microsecond accuracy.
 */
double pChrono::now() const
{
  auto tp = std::chrono::system_clock::now();
  auto us = std::chrono::time_point_cast<std::chrono::microseconds>(tp);
  return 1.0e-6*us.time_since_epoch().count();
}


/*! Start the chronometer, i.e., set the start time to the current seconds from
  January 1, 1970.
 */
double pChrono::start()
{
  m_start = now();
  return m_start;
}


/*! Stop the chronometer, i.e., set the stop time to the current seconds from
  January 1, 1970.
 */
double pChrono::stop()
{
  m_stop = now();
  return m_stop;
}


/*! Return the number of seconds elapsed since the start.
 */
double pChrono::split() const
{
  return now() - m_start;
}


/*! Convert a double-precision floating point number representing the seconds
  from January 1, 1970 into a datetime string (including the fractional part
  of the seconds up to the microsecond).
 */
std::string pChrono::double2datetime(double t) const
{
  // Split seconds and microseconds.
  long int s = static_cast<long int> (t);
  int us = round(1.e6*(t - s));
  // Use the put_time manipulator to format the datetime.
  std::stringstream datetime("");
  datetime << std::put_time(std::localtime(&s), "%b %d, %Y @ %T") << "." << us;
  return datetime.str();
}


/*! Terminal formatting---this prints out both the datetime and the seconds
  from January 1, 2016 of the start time.
 */
std::string pChrono::startDateTime() const
{
  std::stringstream datetime("");
  datetime << double2datetime(m_start) << " (" << std::fixed << m_start
	   << std::defaultfloat << " s since January 1, 1970)";
  return datetime.str();
}


/*! Terminal formatting---this prints out both the datetime and the seconds
  from January 1, 2016 of the stop time.
 */
std::string pChrono::stopDateTime() const
{
  std::stringstream datetime("");
  datetime << double2datetime(m_stop) << " (" << std::fixed << m_stop
	   << std::defaultfloat << " s since January 1, 1970)";
  return datetime.str();
}


/*! Terminal formatting---this prints out both the datetime and the seconds
  from January 1, 2016.
 */
std::ostream& pChrono::fillStream(std::ostream& os) const
{
  double t = now();
  os << double2datetime(t) << " (" << std::fixed << t << std::defaultfloat
     << " s since January 1, 1970)";
  return os;
}
