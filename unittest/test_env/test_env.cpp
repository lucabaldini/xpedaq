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

#include "xpolenv.h"

int main()
{
  std::cout << xpolenv::kDaqRootDirPath << std::endl;
  std::cout << xpolenv::kDirSeparator << std::endl;
  std::cout << xpolenv::kNullPath << std::endl;
  std::cout << xpolenv::kDefaultRunIdFilePath << std::endl;
  std::cout << xpolenv::kDefaultPreferencesFilePath << std::endl;
  std::cout << xpolenv::kDaqConfigDirPath << std::endl;
  std::cout << xpolenv::kDefaultConfigFilePath << std::endl;
  std::cout << xpolenv::kDaqGuiDirPath << std::endl;
  std::cout << xpolenv::kDaqPixmapsDirPath << std::endl;
  std::cout << xpolenv::kDaqDataDirPath << std::endl;
  std::cout << xpolenv::kDaqLogDirPath << std::endl;
}
