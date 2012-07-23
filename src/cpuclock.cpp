#include "cpuclock.hpp"
#include <cstdio>

unsigned CpuClock::getMHzCpuClock() {
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
