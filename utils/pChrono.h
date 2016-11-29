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


#ifndef PCHRONO_H
#define PCHRONO_H


#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <math.h>


/*! Simple timing facility.

This is essentially a simple timing facility that can be started and stopped
as if it was a chronometer.

All the underlying times are measured since January 1, 1970 and stored as
 double-precision floating point numbers, with a theoretical microsecond
accuracy (although I am sure the least significant digits are not accurate).
*/


class pChrono
{
  public:

    pChrono();
    double now() const;
    double start();
    double stop();
    double split() const;

    double startSeconds() const {return m_start;}
    double stopSeconds() const {return m_start;}
    std::string startDateTime() const {return double2datetime(m_start);};
    std::string stopDateTime() const {return double2datetime(m_stop);};

    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, const pChrono& chrono)
    {return chrono.fillStream(os);}


  private:

    double m_start;
    double m_stop;
    std::string double2datetime(double t) const;
};

#endif //PCHRONO_H
