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

#ifndef XPEREGUSERPREFERENCES_H
#define XPEREGUSERPREFERENCES_H

#include <iostream>

#include "xpedaqutils.h"
#include "xpolio.h"


class xperegUserPreferences
{

  public:

    xperegUserPreferences(std::string filePath);

    unsigned short m_pixelAddressX;
    unsigned short m_pixelAddressY;
    unsigned short m_configuration;
    bool m_randomShuffle;
    int m_numReadouts;
    int m_readoutInterval;
    bool m_dataFileEnabled;
    std::string m_outputFolder;
    bool m_logFileEnabled;
    int m_loggerTerminalLevel;
    int m_loggerDisplayLevel;
  
    // Read/write to/from file.
    void writeToFile(std::string filePath);
    void readFromFile(std::string filePath);

    // Terminal formatting.
    std::ostream& fillStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os,
				    const xperegUserPreferences& preferences)
    {return preferences.fillStream(os);}

};

#endif //XPEREGUSERPREFERENCES_H
