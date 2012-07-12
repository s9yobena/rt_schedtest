#ifndef PARTITIONNED_TEST_H
#define PARTITIONNED_TEST_H

#include "schedtest.hpp"

class PartitionnedTest : public SchedTest{

protected:
  void drawTaskSetSafeApprox();

public :
  PartitionnedTest();
  ~PartitionnedTest();

  int makeSchedTest();
};


#endif
