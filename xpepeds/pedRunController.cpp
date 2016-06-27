#include "pedRunController.h"


pedRunController::pedRunController(std::string configFilePath,
                                   std::string preferencesFilePath,
                                   std::string trgMaskFilePath) : 
                                   pRunController(configFilePath,
                                                  preferencesFilePath,
                                                  trgMaskFilePath,
                                                  true)
{
  m_pedestalMap = new PedestalsMap();
  connect (m_dataCollector, SIGNAL(blockRead(const pDataBlock&)),
          this, SLOT(readDataBlock(const pDataBlock&)));
}                                   


void pedRunController::readDataBlock(const pDataBlock &p)
{
  for (unsigned int evt = 0; evt < p.numEvents(); ++evt)
  {
    unsigned int xMin = p.xmin(evt);
    unsigned int xMax = p.xmax(evt);
    unsigned int yMin = p.ymin(evt);    
    unsigned int nPixel = p.numPixels(evt);
    unsigned int nCol = xMax - xMin + 1;
    for (unsigned int index = 0; index < nPixel; ++index)
    {
      double height = p.pixelCounts(evt, index);
      unsigned int x = xMin + index % nCol;
      unsigned int y = yMin + index / nCol;
      m_pedestalMap -> fill(x, y, height);
    }
  }
}


void pedRunController::randomFilling(int nEvents)
{
  /* Generate some fake data. This is only for testing and should be removed
  as soon as we are able to generate sensible input values */

  std::cout << "Fake random data generation" << std::endl; 
  
  double baseline = 2.;
  double fluctuation_range = 2.;
  
  //initialize random generator
  std::random_device rd; // Use a hardware entropy source if available, otherwise use PRNG
  std::mt19937 mersenne(rd()); // initialize a mersenne twister with a random seed
  
  //std::srand(static_cast<unsigned int>(time(0)));
  static const double fraction = 1.0 / (static_cast<double>(mersenne.max())
                                                                        + 1.0);
  
  for (unsigned int xIndex=0; xIndex < pedestals::kNx; xIndex++)
  {
    for (unsigned int yIndex=0; yIndex < pedestals::kNy; yIndex++)
    {
      for (int i = 0; i < nEvents; ++i)
      {
        double random_fluctuation = (2. * static_cast<double>(mersenne())
                                     * fraction - 1.) * fluctuation_range;
        m_pedestalMap -> fill(xIndex, yIndex, baseline + random_fluctuation);
      }
    }
  }
  std::cout << "End of random generation" << std::endl;
}
