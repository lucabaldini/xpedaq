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

#include "pTriggerMask.h"


/*!
 */
pTriggerMask::pTriggerMask()
{
  m_mask = new TriggerMask_t();
}


/*!
 */
pTriggerMask::pTriggerMask(std::string filePath)
{
  m_mask = new TriggerMask_t();
  readFromFile(filePath);
}


/*!
 */
void pTriggerMask::add(unsigned short x, unsigned short y)
{
  bool insertError = false;
  for (const auto &chan : *m_mask) {
    /*if ((chan.first == x) & (chan.second == y)) {
      *xpollog::kError << "Duplicate trigger mask entry (" << x << ", " << y
		       << "), skipping..." << endline;
      insertError = true;
      break;
    }*/
  }
  if (!insertError) { 
    m_mask->push_back(std::make_pair(x, y));
  }
}


/*!
 */
void pTriggerMask::remove(unsigned short x, unsigned short y)
{
  TriggerMask_t::iterator chan;
  for (chan = m_mask->begin(); chan != m_mask->end(); chan++) {
    if (((*chan).first == x) & ((*chan).second == y)) {
      m_mask->erase(chan);
      break;
    }
  }
}


/*!
 */
void pTriggerMask::writeToFile(std::string filePath) const
{
  *xpollog::kInfo << "Writing trigger mask to " << filePath <<
    "... " << endline;
  std::ofstream *outputFile = xpolio::kIOManager->openOutputFile(filePath);
  for (const auto &chan : *m_mask) {
    unsigned short x = chan.first;
    unsigned short y = chan.second;
    xpolio::kIOManager->write(outputFile, x, false);
    xpolio::kIOManager->write(outputFile, " ", false);
    xpolio::kIOManager->write(outputFile, y);
  }
  xpolio::kIOManager->closeOutputFile(outputFile);
}


/*!
 */
void pTriggerMask::readFromFile(std::string filePath)
{
  *xpollog::kInfo << "Reading trigger mask from " << filePath <<
    "... " << endline;
  std::ifstream *inputFile = xpolio::kIOManager->openInputFile(filePath);
  unsigned short x;
  unsigned short y;
  while (!inputFile->eof()) {
    x = xpolio::kIOManager->readUnsignedShort(inputFile);
    if (!inputFile->eof()) {
      y = xpolio::kIOManager->readUnsignedShort(inputFile);
      add(x, y);
    }
  }
  xpolio::kIOManager->closeInputFile(inputFile);
}


std::ostream& pTriggerMask::fillStream(std::ostream& os) const
{
  os << xpedaqutils::title("Trigger mask", true);
  for (const auto &chan : *m_mask) {
    unsigned short x = chan.first;
    unsigned short y = chan.second;
    os << std::setw(3) << x << "   " << y << std::endl;
  }
  os << xpedaqutils::hline();
  return os;
}
