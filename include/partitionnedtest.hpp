#ifndef PARTITIONNED_TEST_H
#define PARTITIONNED_TEST_H

#include "schedtest.hpp"
#include <vector>

using namespace std;

class PartitionnedTest : public SchedTest{

protected:
  
  vector<int> cpus;
  
  void drawTaskSetSafeApprox();
  
public :
  PartitionnedTest();
  ~PartitionnedTest();

  void setCpus(vector<int>);

  int makeSchedTest();
};


#endif
