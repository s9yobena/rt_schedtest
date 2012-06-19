#ifndef DENSITYTEST_H
#define DENSITYTEST_H

#include "schedtest.hpp"

class DensityTest : public SchedTest{

public :
  DensityTest();
  ~DensityTest();
  int makeSchedTest();

};


#endif
