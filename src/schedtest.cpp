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
  // IMPORTANT: We are making the assumption that the clock
  // speed found in /proc/cpuinfo is the maximum clock speed.
  // this should be true since freqeuncy scaling is disabled 
  // under LITMUS
  FILE * fp;
  char line[128];
  unsigned cpuspeed;
  fp = popen("/bin/cat /proc/cpuinfo | grep 'cpu MHz' | sed 's/[^0-9\.]//g' ","r");
  fgets(line,sizeof line,fp);
  sscanf(line,"%u",&cpuspeed);
  pclose(fp);
  return cpuspeed;
}
