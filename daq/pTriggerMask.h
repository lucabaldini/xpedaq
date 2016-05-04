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

#ifndef PTRIGGERMASK_H
#define PTRIGGERMASK_H


#include <vector>
#include <iostream>
#include <iomanip>

#include "xpollog.h"
#include "xpolio.h"
#include "xpedaqutils.h"


typedef std::vector< std::pair<unsigned short, unsigned short> > TriggerMask_t;


class pTriggerMask
{

 public:

  ///\brief Do-nothing constructor.
  pTriggerMask();

  ///\brief Constructor from file.
  pTriggerMask(std::string filePath);

  ///\brief Destructor.
  ~pTriggerMask() {;}

  ///\brief Return the underlying TriggerMask_t object.
  TriggerMask_t *mask() const {return m_mask;}

  ///\brief Add a channel to the mask.
  void add(unsigned short x, unsigned short y);

  ///\brief Remove a channel from the mask;
  void remove(unsigned short x, unsigned short y);

  ///\brief Return the number of masked channels.
  int size() const {return m_mask->size();}

  ///\brief Write the mask to file.
  void writeToFile(std::string filePath);

  ///\brief Read the mask from file.
  void readFromFile(std::string filePath);

  // Terminal formatting.
  std::ostream& fillStream(std::ostream& os) const;
  friend std::ostream& operator<<(std::ostream& os, const pTriggerMask& mask)
    {return mask.fillStream(os);}

 protected:

  
 private:

  ///\brief Underlying TriggerMask_t object.
  TriggerMask_t *m_mask;
  
};

#endif //PTRIGGERMASK_H
