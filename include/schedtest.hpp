#ifndef SCHEDTEST_H
#define SCHEDTEST_H

#include <cstdlib>
#include <cstdio>

#include "taskset.hpp"
#include "overhead.hpp"

#define __US_PER_MS 1000

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
  int get_nbr_cpus();


 public :
  SchedTest();
  ~SchedTest();
  void setTaskSet(TaskSet*);
  void setOverhead(Overhead*);  
  int densityTest();
  int makeSchedTest();

};


#endif
