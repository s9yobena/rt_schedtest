#ifndef SCHEDTEST_H
#define SCHEDTEST_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include "taskset.hpp"
#include "overhead.hpp"

#define __CPU_CLOCK_MHz 10000.0
#define __Hz_PER_MHz 1000000.0
#define __MS_PER_S 1000.0

#ifndef LITMUS_H
#define __NS_PER_MS 1000000
#endif

using namespace std;

enum SchedTestName {
  DENSITY_TEST,
  BAKERS_TEST
};

class Overhead;

class SchedTest{

  SchedTestName schedTestName;
  TaskSet *taskSet;
  Overhead *overhead;

  int nbr_cpu;
  long double msPerCycle;
  long double getMsPerCycle();
  long double getNsPerCycle();


 public :
  SchedTest();
  ~SchedTest();
  void setTaskSet(TaskSet*);
  void setOverhead(Overhead*);  
  int densityTest();
  int makeSchedTest();

};


#endif
