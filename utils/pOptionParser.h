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

  /// \brief Destructor.
  ~pOptionParser() {}

  /// \brief Add an option to the option map.
  void addOption(std::string longopt, char shortopt, pOption option);

  /// \brief Add an option to the option map.
  template <class T> void addOption(std::string longopt, char shortopt,
                                    T defaultValue, std::string help)
    {
      pOption option(longopt, shortopt, pVariant(defaultValue), help);
      addOption(longopt, shortopt, option);
    }

  /// \brief Add an option with no default value.
  template <class T> void addOption(std::string longopt, char shortopt,
                                    std::string help, bool required);

  /// \brief Add a non-required option with no degault value
  template <class T> void addOption(std::string longopt, char shortopt,
                                    std::string help);

  /// \brief Return the *value* of a given option.
  template <class T> T option(std::string longopt)
    { 
      std::map<std::string, pOption>::iterator item = mapIter(longopt);
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
  /// \brief Check whether all the required options have been set.
  void checkOptions() const ;

  /// \brief Return the long option corresponding to a short option.
  std::string longOption(char shortopt) const;

  /// \brief Access map elements by long name.
  std::map<std::string, pOption>::iterator mapIter(std::string longopt);

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



template <> inline void pOptionParser::addOption<bool>(std::string longopt,
                                                        char shortopt,
                                                        std::string help)
{
  pOptionParser::addOption<bool>(longopt, shortopt, false, help);
}


template <> inline void pOptionParser::addOption<int>(std::string longopt,
                                                       char shortopt,
                                                       std::string help,
                                                       bool required)
{
  pOption option(longopt, shortopt, pVariant(0), help, required, false);
  pOptionParser::addOption(longopt, shortopt, option);
}


template <> inline void pOptionParser::addOption<double>(std::string longopt,
                                                          char shortopt,
                                                          std::string help,
                                                          bool required)
{
  pOption option(longopt, shortopt, pVariant(0.), help, required, false);
  pOptionParser::addOption(longopt, shortopt, option);
}


template <> inline void pOptionParser::addOption<std::string>(std::string longopt,
                                                               char shortopt,
                                                               std::string help,
                                                               bool required)
{
  pOption option(longopt, shortopt, pVariant(std::string("")), help,
                  required, false);
  pOptionParser::addOption(longopt, shortopt, option);
}


#endif //POPTIONPARSER_H
