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

#include "pIOManager.h"

pIOManager::pIOManager()
{

}

std::ifstream *pIOManager::openInputFile(const char* filePath, bool binary)
{
  std::ifstream *inputFile = new std::ifstream();
  std::_Ios_Openmode mode = std::ios_base::in;
  if (binary){
    mode |= std::ios_base::binary;
  }
  inputFile->open(filePath, mode);
  if (!*inputFile){
    *xpollog::kError << "Could not open " << filePath <<
      " in read mode. Aborting..." << endline;
    exit(1);
  }
  return inputFile;
}

std::ifstream *pIOManager::openInputFile(std::string filePath, bool binary)
{
  return openInputFile(filePath.c_str(), binary);
}

unsigned short int pIOManager::readUnsignedShort(std::ifstream *inputFile,
						 bool endline)
{
  short unsigned int item;
  *inputFile >> item;
  if (endline){
    inputFile->ignore(1, '\n');
  }
  return item;
}

unsigned char pIOManager::readUnsignedChar(std::ifstream *inputFile,
					   bool endline)
{
  unsigned char item;
  *inputFile >> item;
  if (endline){
    inputFile->ignore(1, '\n');
  }
  return item;
}

int pIOManager::readInteger(std::ifstream *inputFile, bool endline)
{
  int item;
  *inputFile >> item;
  if (endline){
    inputFile->ignore(1, '\n');
  }
  return item;
}

unsigned int pIOManager::readUnsignedInt(std::ifstream *inputFile,
                                         bool endline)
{
  unsigned int item;
  *inputFile >> item;
  if (endline){
    inputFile->ignore(1, '\n');
  }
  return item;
}

double pIOManager::readDouble(std::ifstream *inputFile, bool endline)
{
  double item;
  *inputFile >> item;
  if (endline){
    inputFile->ignore(1, '\n');
  }
  return item;
}

std::string pIOManager::readLine(std::ifstream *inputFile, bool endline)
{
  std::string item;
  getline(*inputFile, item, '\n');
  if (endline){
    inputFile->ignore(1, '\n');
  }
  return item;
}

void pIOManager::skipLine(std::ifstream *inputFile)
{
  inputFile->ignore(1000, '\n');
}

unsigned short int pIOManager::getUnsignedShort(std::string filePath)
{ 
  std::ifstream *inputFile = openInputFile(filePath);
  short unsigned int item = readUnsignedShort(inputFile);
  closeInputFile(inputFile);
  return item;
}

int pIOManager::getInteger(std::string filePath)
{
  std::ifstream *inputFile = openInputFile(filePath);
  int item = readInteger(inputFile);
  closeInputFile(inputFile);
  return item;
}

double pIOManager::getDouble(std::string filePath)
{
  std::ifstream *inputFile = openInputFile(filePath);
  double item = readDouble(inputFile);
  closeInputFile(inputFile);
  return item;
}

std::string pIOManager::getLine(std::string filePath)
{
  std::ifstream *inputFile = openInputFile(filePath);
  std::string item = readLine(inputFile);
  closeInputFile(inputFile);
  return item;
}

void pIOManager::closeInputFile(std::ifstream *inputFile)
{
  inputFile->close();
  delete inputFile;
}

std::ofstream *pIOManager::openOutputFile(const char* filePath, bool append,
					  bool binary)
{
  std::ofstream *outputFile = new std::ofstream();
  std::_Ios_Openmode mode = std::ios_base::out;
  if (append){
    mode |= std::ios_base::app;
  }
  if (binary){
    mode |= std::ios_base::binary;
  }
  outputFile->open(filePath, mode);
  if (!*outputFile){
    *xpollog::kError << "Could not open " << filePath <<
      " in write mode. Aborting..." << endline;
    exit(1);
  }
  return outputFile;
}

std::ofstream *pIOManager::openOutputFile(std::string filePath, bool append,
					  bool binary)
{
  return openOutputFile(filePath.c_str(), append, binary);
}

void pIOManager::writeEndline(std::ofstream *outputFile)
{
  *outputFile << std::endl;
}

void pIOManager::write(std::ofstream *outputFile, unsigned short int item,
		       bool endline)
{
  *outputFile << item;
  if (endline){
    *outputFile << std::endl;
  }
}

void pIOManager::write(std::ofstream *outputFile, int item, bool endline)
{
  *outputFile << item;
  if (endline){
    *outputFile << std::endl;
  }
}

void pIOManager::write(std::ofstream *outputFile, unsigned int item, bool endline)
{
  *outputFile << item;
  if (endline){
    *outputFile << std::endl;
  }
}

void pIOManager::write(std::ofstream *outputFile, double item, bool endline)
{
  *outputFile << item;
  if (endline){
    *outputFile << std::endl;
  }
}

void pIOManager::write(std::ofstream *outputFile, std::string item,
		       bool endline)
{
  *outputFile << item;
  if (endline){
    *outputFile << std::endl;
  }
}

void pIOManager::write(std::ofstream *outputFile, unsigned char* item,
		       int length)
{ 
  outputFile->write((const char*)item, length);
}

void pIOManager::put(std::string filePath, unsigned short int item)
{
  std::ofstream *outputFile = openOutputFile(filePath);
  write(outputFile, item, true);
  closeOutputFile(outputFile);
}

void pIOManager::put(std::string filePath, int item)
{
  std::ofstream *outputFile = openOutputFile(filePath);
  write(outputFile, item, true);
  closeOutputFile(outputFile);
}

void pIOManager::put(std::string filePath, unsigned int item)
{
  std::ofstream *outputFile = openOutputFile(filePath);
  write(outputFile, item, true);
  closeOutputFile(outputFile);
}

void pIOManager::put(std::string filePath, double item)
{
  std::ofstream *outputFile = openOutputFile(filePath);
  write(outputFile, item, true);
  closeOutputFile(outputFile);
}

void pIOManager::put(std::string filePath, std::string item)
{
  std::ofstream *outputFile = openOutputFile(filePath);
  write(outputFile, item, true);
  closeOutputFile(outputFile);
}

void pIOManager::append(std::string filePath, unsigned short int item)
{
  std::ofstream *outputFile = openOutputFile(filePath, true, false);
  write(outputFile, item, true);
  closeOutputFile(outputFile);
}

void pIOManager::append(std::string filePath, int item)
{
  std::ofstream *outputFile = openOutputFile(filePath, true, false);
  write(outputFile, item, true);
  closeOutputFile(outputFile);
}

void pIOManager::append(std::string filePath, unsigned int item)
{
  std::ofstream *outputFile = openOutputFile(filePath, true, false);
  write(outputFile, item, true);
  closeOutputFile(outputFile);
}

void pIOManager::append(std::string filePath, double item)
{
  std::ofstream *outputFile = openOutputFile(filePath, true, false);
  write(outputFile, item, true);
  closeOutputFile(outputFile);
}

void pIOManager::append(std::string filePath, std::string item)
{
  std::ofstream *outputFile = openOutputFile(filePath, true, false);
  write(outputFile, item, true);
  closeOutputFile(outputFile);
}

void pIOManager::closeOutputFile(std::ofstream *outputFile)
{
  outputFile->close();
  delete outputFile;
}
