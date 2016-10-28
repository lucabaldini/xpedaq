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

#ifndef PIOMANAGER_H
#define PIOMANAGER_H

#include <iostream>
#include <fstream>
#include <limits>
#include "xpollog.h"
#include <cstdlib>

class pIOManager
{

 public:
  pIOManager();
  ~pIOManager() {;}
  std::ifstream *openInputFile(const char* filePath, bool binary = false);
  std::ifstream *openInputFile(std::string filePath, bool binary = false);
  unsigned short int readUnsignedShort(std::ifstream *inputFile,
  				       bool endline = true);
  unsigned char readUnsignedChar(std::ifstream *inputFile,
				 bool endline = true);
  int readInteger(std::ifstream *inputFile, bool endline = true);
  unsigned int readUnsignedInt(std::ifstream *inputFile, bool endline = true);
  double readDouble(std::ifstream *inputFile, bool endline = true);
  std::string readLine(std::ifstream *inputFile, bool endline = true);
  bool readBool(std::ifstream *inputFile, bool endline = true);
  void skipLine(std::ifstream *inputFile);
  unsigned short int getUnsignedShort(std::string filePath);
  int getInteger(std::string filePath);
  double getDouble(std::string filePath);
  std::string getLine(std::string filePath);
  void closeInputFile(std::ifstream *inputFile);
  std::ofstream *openOutputFile(const char* filePath, bool append = false,
				bool binary = false);
  std::ofstream *openOutputFile(std::string filePath, bool append = false,
				bool binary = false);
  void writeEndline(std::ofstream *outputFile);
  void write(std::ofstream *outputFile, unsigned short int item,
	     bool endline = true);
  void write(std::ofstream *outputFile, int item, bool endline = true);
  void write(std::ofstream *outputFile, unsigned int item, bool endline = true);
  void write(std::ofstream *outputFile, double item, bool endline = true);
  void write(std::ofstream *outputFile, std::string item, bool endline = true);
  void write(std::ofstream *outputFile, unsigned char* item, int length);
  void put(std::string filePath, unsigned short int item);
  void put(std::string filePath, int item);
  void put(std::string filePath, unsigned int item);
  void put(std::string filePath, double item);
  void put(std::string filePath, std::string item);
  void append(std::string filePath, unsigned short int item);
  void append(std::string filePath, int item);
  void append(std::string filePath, unsigned int item);
  void append(std::string filePath, double item);
  void append(std::string filePath, std::string item);
  void closeOutputFile(std::ofstream *outputFile);

 protected:

 private:
};

#endif
