#ifndef OVERHEAD_H
#define OVERHEAD_H

#include <cstdlib>
#include <cstdio>

#include "schedtest.hpp"

typedef unsigned long long overhead_t;

class SchedTest;

class Overhead {

private:

  Overhead();
  Overhead(const Overhead&);
  Overhead& operator=(const Overhead&);
  static Overhead* overheadInstance;

  static SchedTest *schedTest;

  overhead_t CXS;
  overhead_t SCHED;
  overhead_t SCHED2;
  overhead_t RELEASE;
  overhead_t RELEASE_LATENCY;
  overhead_t SEND_RESCHED;
  overhead_t CPMD;



public :  

  static Overhead* getInstance();
  void updateCXS(overhead_t);
  void updateSCHED(overhead_t);
  void updateSCHED2(overhead_t);
  void updateRELEASE(overhead_t);
  void updateRELEASE_LATENCY(overhead_t);
  void updateSEND_RESCHED(overhead_t);
  void updateCPMD(overhead_t);
  void updateSchedTest();
  overhead_t getCXS();
  overhead_t getSCHED();
  overhead_t getSCHED2();
  overhead_t getRELEASE();  
  overhead_t getRELEASE_LATENCY();
  overhead_t getSEND_RESCHED();
  overhead_t getCPMD();

  void setParameters(const CmdlParser&);

  void setSchedTestObserver(SchedTest*);

};

#endif
