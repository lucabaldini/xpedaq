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


#ifndef POPTIONPARSER_H
#define POPTIONPARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <exception>
#include <stdlib.h>

#include "pOption.h"


class pOptionParser
{
 public:
  /// \brief Constructor
  pOptionParser(std::string intent, std::string usage);

  /// \brief Return the option object for a given long name.
  pOption option(std::string longName);

  /// \brief Return whether the option for a given long name is set.
  bool optionSet(std::string longName);

  /// \brief Add an option to the option map.
  void addOption(std::string longName, char shortName, pOption option);

  /// \brief Add an option with no default value.
  template <class T> void addOption(std::string longName, char shortName,
                                    std::string help);

  /// \brief Return the *value* of a given option.
  template <class T> T value(std::string longName)
    { 
      std::map<std::string, pOption>::iterator item = mapIter(longName);
      return (item->second).value<T>();
    }

  /// \brief Return the argument list.
  std::vector<std::string> arguments() const {return m_argumentVec;}

  /// \brif Return the number of arguments.
  unsigned int numArguments() const {return m_argumentVec.size();}

  /// \brief Print out a parser error.
  void parseError(std::string msg) const;

  /// \brief Print out help and exit.
  void help() const;

  /// \brief Parse the arguments.
  void parse(int argc, char* argv[]);

  /// \brief Standard output.
  std::ostream& fillStream(std::ostream& os) const;

  /// \brief Standard output.
  friend std::ostream& operator<<(std::ostream& os, const pOptionParser& p)
  { return p.fillStream(os); }

 private:
  /// \brief
  void checkOptions() const ;

  /// \brief Return the long option corresponding to a short option.
  std::string longName(char shortName) const;

  /// \brief Access map elements by long name.
  std::map<std::string, pOption>::iterator mapIter(std::string longName);

  /// \brief The program intent.
  std::string m_intent;

  /// \brief The program usage.
  std::string m_usage;

  /// \brief The map storing the corrispondence between the long and the
  /// short forms of the options.
  std::map<char, std::string> m_short2longMap;

  /// \brief The underlying option map.
  std::map<std::string, pOption> m_optionMap;

  /// \brief The underlying argument list.
  std::vector<std::string> m_argumentVec;
};



template <> inline void pOptionParser::addOption<bool>(std::string longName,
						       char shortName,
						       std::string help)
{
  pOption option(longName, shortName, pVariant(false), help);
  pOptionParser::addOption(longName, shortName, option);
}


template <> inline void pOptionParser::addOption<int>(std::string longName,
						      char shortName,
						      std::string help)
{
  pOption option(longName, shortName, pVariant(0), help);
  pOptionParser::addOption(longName, shortName, option);
}


template <> inline void pOptionParser::addOption<double>(std::string longName,
							 char shortName,
							 std::string help)
{
  pOption option(longName, shortName, pVariant(0.), help);
  pOptionParser::addOption(longName, shortName, option);
}


template <> inline void pOptionParser::addOption<std::string>(std::string longName,
							      char shortName,
							      std::string help)
{
  pOption option(longName, shortName, pVariant(std::string("")), help);
  pOptionParser::addOption(longName, shortName, option);
}


#endif //POPTIONPARSER_H
