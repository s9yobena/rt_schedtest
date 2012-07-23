#ifndef SCHEDTEST_H
#define SCHEDTEST_H

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include "taskset.hpp"
#include "overhead.hpp"
#include "rttypes.h"		// for enum SchedTestName

#define __Hz_PER_MHz 1000000.0
#define __MS_PER_S 1000.0

#define TIMER_FREQ_HZ 1000

#ifndef LITMUS_H
#define __NS_PER_MS 1000000
#endif

using namespace std;

class Overhead;

class SchedTest{

protected:

  SchedTestName schedTestName;
  TaskSet *taskSet;
  Overhead *overhead;
  unsigned mhzCpuClock;

  int nbr_cpu;
  long double msPerCycle;
  long double getMsPerCycle();
  long double getNsPerCycle();

  long double getNsTimerPeriod();
  unsigned getMHzCpuClock();
  
public:
  SchedTest();
  ~SchedTest();
  void setTaskSet(TaskSet*);
  void setOverhead(Overhead*);  
  void setNbrCpus(unsigned);
  void setMHzCpuClock(unsigned);
  
};


#endif
