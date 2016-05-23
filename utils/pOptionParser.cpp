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


#include "pOptionParser.h"


/*!
 */
pOptionParser::pOptionParser(std::string intent, std::string usage) :
  m_intent(intent),
  m_usage(usage)
{
  addOption<bool>("help", 'h', "Print help and exit");
}


/*!
 */
pOption pOptionParser::option(std::string longName)
{
  std::map<std::string, pOption>::iterator item = m_optionMap.find(longName);
  if (item == m_optionMap.end()) {
    parseError("Unrecognized long option --" + longName + ".");
  }
  return item->second;
}


/*!
 */
bool pOptionParser::optionSet(std::string longName)
{
  return option(longName).set();
}


void pOptionParser::addOption(std::string longName, char shortName,
			      pOption option)
{
  m_optionMap.insert(std::make_pair(longName, option));
  m_short2longMap.insert(std::make_pair(shortName, longName));
}


std::string pOptionParser::longName(char shortName) const
{
  std::map<char, std::string>::const_iterator item =
    m_short2longMap.find(shortName);
  if (item == m_short2longMap.end()) {
    std::cout << shortName << std::endl;
    parseError("Unrecognized short option -" + std::string(1, shortName) + ".");
  }
  return item->second;
}


void pOptionParser::parseError(std::string msg) const
{
  std::cout << *this << std::endl << "Parsing error: " << msg << std::endl;
  exit(1);
}


void pOptionParser::help() const
{
  std::cout << *this << std::endl;
  exit(0);
}


void pOptionParser::checkOptions() const 
{
  //std::map<std::string, pOption>::const_iterator item;
  //for (item = m_optionMap.begin(); item != m_optionMap.end(); item++) {
  //  pOption option = item->second;
  //}
}


std::map<std::string, pOption>::iterator
pOptionParser::mapIter(std::string longName)
{
  std::map<std::string, pOption>::iterator item = m_optionMap.find(longName);
  if (item == m_optionMap.end()) {
    parseError("Unrecognized long option --" + longName + ".");
  }
  return item;
}


void pOptionParser::parse(int argc, char* argv[])
{
  std::string token;
  std::string optName;
  char* endptr;
  for (int i = 1; i < argc; i++) {
    token = argv[i];
    // Need help?
    if (token == "-h" || token == "--help") {
      help();
    }
    
    // Resolve the mapping between long and short options.
    if (token.substr(0,2) == "--") {
      optName = token.substr(2, token.length());
    } else if (token.substr(0,1) == "-") {
      optName = longName(argv[i][1]);
    }
    
    if (token.substr(0,1) == "-") {
      // Is the token an option...      
      std::map<std::string, pOption>::iterator item = mapIter(optName);
      if (item->second.type() == pVariant::Boolean) {
        item->second.setValue(pVariant(true));
      } else {
        i++;
        if (i >= argc) {
          parseError("Missing argument for option " + optName + ".");
        }
        token = argv[i];
        if (item->second.type() == pVariant::Literal) {
          item->second.setValue(pVariant(token));
        }
        if (item->second.type() == pVariant::Integer) {
	  int val = strtol(token.c_str(), &endptr, 10);
	  if (*endptr) {
	    parseError("Invalid argument '" + token + "' to " + optName + ".");
	  }
	  item->second.setValue(pVariant(val));
        }
        if (item->second.type() == pVariant::Floating) {
	  double val = strtod(token.c_str(), &endptr);
	  if (*endptr) {
	    parseError("Invalid argument '" + token + "' to " + optName + "." );
	  }
	  item->second.setValue(pVariant(val));
        }
      }      
    } else {
      // ... or an argument?
      m_argumentVec.push_back(token); 
    }
  }
  checkOptions();
}


std::ostream& pOptionParser::fillStream(std::ostream& os) const
{
  os << m_intent << "\n\n"
     << "Usage: " << m_usage << "\n\n"
     << "Recognized options: " << std::endl;
  std::map<std::string, pOption>::const_iterator item;
  for (item = m_optionMap.begin(); item != m_optionMap.end(); item++) {
    os << "  " << item->second << std::endl;
  }
  return os;
}
