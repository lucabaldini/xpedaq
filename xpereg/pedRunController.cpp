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
    adc_count_t height = 0;
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
