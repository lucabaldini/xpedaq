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


#ifndef PVARIANT_H
#define PVARIANT_H

#include <iostream>
#include <typeinfo>
#include <string>



class pVariant
{
  
 public:
  enum VariantType {
    Boolean,
    Integer,
    Floating,
    Literal
  };
  
  /// \brief Constructor setting the variant value.
  template <class T> pVariant(T value);

  /// \brief Return the type currently held in the variable.
  VariantType type() const {return m_type;}

  /// \brief Return a string representation of the variant type.
  std::string typeString() const;

  /// \brief Forward declaration for the specialized template functions.
  template<class T> T value() const;

  /// \brief Standard output.
  std::ostream& fillStream(std::ostream& os) const;

  /// \brief Standard output.
  friend std::ostream& operator<<(std::ostream& os, const pVariant& v)
    {return v.fillStream(os);}

 private:
  /// \brief The basic underlying class member storing the variable value.
  union VariantData {
    bool b;
    int i;
    double d;
  } m_data;
  
  /// \brief The member variable for the string data of the variant.
  std::string m_string;
  
  /// brief The type currently held in the variant.
  VariantType m_type;
};


template <> inline pVariant::pVariant<bool>(bool value)
  {m_data.b = value; m_type = Boolean;}

template <> inline pVariant::pVariant<int>(int value)
  {m_data.i = value; m_type = Integer;}

template <> inline pVariant::pVariant<double>(double value)
  {m_data.d = value; m_type = Floating;}

template <> inline pVariant::pVariant<std::string>(std::string value)
  {m_string = value; m_type = Literal;}

template <> inline bool pVariant::value<bool>() const {return m_data.b;}

template <> inline int pVariant::value<int>() const {return m_data.i;}

template <> inline double pVariant::value<double>() const {return m_data.d;}

template <> inline std::string pVariant::value<std::string>() const
  {return m_string;}

#endif //PVARIANT_H
