#include "schedtest.hpp"

SchedTest::SchedTest() {
  this->nbr_cpu = sysconf(_SC_NPROCESSORS_ONLN);
}

SchedTest::~SchedTest() {
}

void SchedTest::setTaskSet(TaskSet *taskSet) {
  this->taskSet = taskSet;
}

void SchedTest::setOverhead(Overhead *overhead) {
  this->overhead = overhead;
}

void SchedTest::setNbrCpus(unsigned _nbrCpus) {

  nbr_cpu = _nbrCpus;
}

long double SchedTest::getMsPerCycle() {

  long double sPerCycle;
  sPerCycle = 1.0/(__Hz_PER_MHz *
		   getMHzCpuClock());
  msPerCycle = sPerCycle * __MS_PER_S;
  return this->msPerCycle;
}

long double SchedTest::getNsPerCycle() {

  return getMsPerCycle() * __NS_PER_MS ;
}

long double SchedTest::getNsTimerPeriod() {
  
  return (1.0/TIMER_FREQ_HZ)*(__NS_PER_MS * __MS_PER_S);
}

unsigned SchedTest::getMHzCpuClock() {
  return mhzCpuClock;
}

void SchedTest::setMHzCpuClock(unsigned _mhzCpuClock) {
  mhzCpuClock = _mhzCpuClock;
}
