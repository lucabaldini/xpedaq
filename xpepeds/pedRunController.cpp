#include "pedRunController.h"


pedRunController::pedRunController(std::string configFilePath,
                                   std::string preferencesFilePath,
                                   std::string trgMaskFilePath,
                                   std::string usrComment) : 
                                   pRunController(configFilePath,
                                                  preferencesFilePath,
                                                  trgMaskFilePath,
                                                  usrComment,
                                                  true)
{
  m_pedestalMap = new PedestalsMap();
  connect (m_dataCollector, SIGNAL(blockRead(const pDataBlock&)),
          this, SLOT(readDataBlock(const pDataBlock&)));
}                                   


void pedRunController::readDataBlock(const pDataBlock &p)
{
  for (unsigned int evt = 0; evt < p.numEvents(); ++evt) {   
    unsigned int x = 1000; //unphysical initialization
    unsigned int y = 1000; //unphysical initialization
    unsigned int height = 0;
    for (unsigned int index = 0; index < p.numPixels(evt); ++index) {
      p.readPixel(evt, index, x, y, height);
      m_pedestalMap -> fill(x, y, height);
    }
  }	
}


void pedRunController::resetPedMap()
{
  m_pedestalMap -> reset();
}


void pedRunController::writeHeader(std::string fileName)
{
  pedHeader_t pedHeader;
  pedHeader.version = FILE_HEADER_VERSION;
  pedHeader.startTime = startSeconds();
  pedHeader.stopTime = stopSeconds();
  pedHeader.nEvents = numEvents();  
  pedHeader.runId = m_runId;
  pedHeader.clockFrequency = m_detectorConfiguration->clockFrequency();
  pedHeader.clockShift = m_detectorConfiguration->clockShift();
  pedHeader.comment = userComment().c_str(); 
  writePedestalHeader(fileName, &pedHeader);
}

void pedRunController::writeMeanFitsImage(std::string fileName)
{
  float values [300*352]; 
  for (int i=0; i<300; ++i){
    for (int j=0; j<352; ++j){
      values[i+300*j] = m_pedestalMap->average(i,j);
    }
  }
  long naxes[2] = {300, 352};
  writeImageExtension(fileName, "PEDMEAN", 2, naxes, values);
}


void pedRunController::writeRmsFitsImage(std::string fileName)
{
  float values [300*352]; 
  for (int i=0; i<300; ++i){
    for (int j=0; j<352; ++j){
      values[i+300*j] = m_pedestalMap->rms(i,j);
    }
  }
  long naxes[2] = {300, 352};
  writeImageExtension(fileName, "PEDRMS", 2, naxes, values);
}


void pedRunController::writeToFile()
{
  std::string filePath = "/home/alberto/xpedaq/fits/prova.fits";
  *xpollog::kInfo << "Writing pedestals map to " << filePath <<
    "... " << endline;
  writeHeader(filePath);
  writeMeanFitsImage(filePath);
  writeRmsFitsImage(filePath);
}
