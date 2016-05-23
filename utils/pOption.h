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


#ifndef POPTION_H
#define POPTION_H

#include <string>
#include <iostream>
#include <iomanip>

#include "pVariant.h"



class pOption
{
 public:
  /// \brief Constructor.
  pOption(std::string longName, char shortName, pVariant value,
	  std::string help, bool required = false, bool initialized = true);

  /// \brief Return the long form of the option.
  std::string longName() const {return m_longName;}

  /// \brief Return the short form of the option.
  char shortName() const {return m_shortName;}

  /// \brief Return true if the option is required.
  bool required() const {return m_required;}

  /// \brief Return true if the option is initialized.
  bool set() const {return m_set;}

  /// \brief Return true if the option is not initialized.
  bool unset() const {return !m_set;}
  
  /// \brief Return the option value.
  template <class T> T value() const {return m_value.value<T>();}

  /// \brief Set the option value.
  void setValue(pVariant value) {m_value = value; m_set = true;}

  /// \brief Return the option type.
  pVariant::VariantType type() const {return m_value.type();}

  /// \brief Return the string version of the option type.
  std::string typeString() const {return m_value.typeString();}

  /// \brief Standard output.
  std::ostream& fillStream(std::ostream& os) const;

  /// \brief Standard output.
  friend std::ostream& operator<<(std::ostream& os, const pOption& o)
    {return o.fillStream(os);}

 private:
  /// \brief The long version of the option.
  std::string m_longName;

  /// \brief The short version of the option.
  char m_shortName;

  /// \brief The option value.
  pVariant m_value;

  /// \brief The help string.
  std::string m_help;

  /// \brief Flag telling whether the option is required.
  bool m_required;

  /// \brief Flag telling whether the option value is set.
  bool m_set;
};

#endif //POPTION_H
