#ifndef PEDRUNCONTROLLER_H
#define PEDRUNCONTROLLER_H

#include <iostream>

#include "pRunController.h"
#include "pedestalsMap.h"


/* Run controller for the pedestal application.
   It is basically identical to its base class pRunController, but with an extra
   member for keeping track of the value and rms of each pixel, and a method for
   filling it from a pDataBlock.
*/ 
   
class pedRunController: public pRunController
{

  Q_OBJECT 
   
  public:
    
    pedRunController(std::string configFilePath,
                     std::string preferencesFilePath,
                     std::string trgMaskFilePath);

    const PedestalsMap& pedMap() const {return (*m_pedestalMap);}

  
  public slots:
  
    void readDataBlock(const pDataBlock &p);
    void randomFilling(int nEvents);

  private:
  
    PedestalsMap *m_pedestalMap;

};

#endif //PEDRUNCONTROLLER_H
