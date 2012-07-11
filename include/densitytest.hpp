#ifndef DENSITYTEST_H
#define DENSITYTEST_H

#include "globaltest.hpp"

class DensityTest : public GlobalTest{

public :
  DensityTest();
  ~DensityTest();

  int makeSchedTest();
};


#endif
