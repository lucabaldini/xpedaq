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


/*
void pedRunController::readDataBlock(const pDataBlock &p)
{	
  //static int counter = 0;
  for (unsigned int index = 0; index < pedestals::kNPedestal; ++index)
  {
	unsigned int height = p.pixelCounts(0, index);
  	if (height == 0) continue;
	unsigned int x = index % pedestals::kNx;
    unsigned int y = index / pedestals::kNx;
	//if (height > 3000)
	//	continue;
	m_pedestalMap -> fill(x, y, static_cast<double>(height));	
    //counter++;	
  }
}
*/


void pedRunController::readDataBlock(const pDataBlock &p)
{
  for (unsigned int evt = 0; evt < p.numEvents(); ++evt) {   
    //int countMM = 0;
    unsigned int x = 1000; //unphysical initialization
    unsigned int y = 1000; //unphysical initialization
    unsigned int height = 0;
    for (unsigned int index = 0; index < p.numPixels(evt); ++index) {
      p.readPixel(evt, index, x, y, height);
      //if (height < 850)
      //{
      //  countMM++;
      //  std::cout << index << std::endl;
      //}
      m_pedestalMap -> fill(x, y, height);
      //if (height > 2000) {
      //std::cout << x << " " << y << " " << height << std::endl;
      //}
    }
    //std::cout << evt << " " << countMM << std::endl;
  }	
}



void pedRunController::resetPedMap()
{
  m_pedestalMap -> reset();
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
