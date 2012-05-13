#ifndef LITMUS_DAEMON_HPP
#define LITMUS_DAEMON_HPP

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sys/wait.h>
#include <iostream>
#include "litmustracingpacket.h"
#include "overhead.hpp"
#include "taskset.hpp"

using namespace std;

#define DEAMON_BUFFER_SIZE 4012

class LitmusDaemon {
  
private:
  LitmusDaemon();
  LitmusDaemon(const LitmusDaemon&);
  LitmusDaemon& operator= (const LitmusDaemon&);
  static LitmusDaemon* litmusDaemonInstance;

  char litmusDaemonBuffer[DEAMON_BUFFER_SIZE];
  int litmusDaemonPipe[2];
  
  void initLitmusDaemonPipe();
  void initDaemon();  
  void initParent();

  Overhead *overhead;
  TaskSet *taskSet;
  void processPacket(struct LitmusTracingPacket*);

  void processLitmusCXSData(overhead_t);
  void processLitmusSCHEDData(overhead_t);
  void processLitmusSCHED2Data(overhead_t);
  void processLitmusRELEASEData(overhead_t);
  void processLitmusRELEASE_LATENCYData(overhead_t);
  void processLitmusSEND_RESCHEDData(overhead_t);
  void processLitmusExecutionCostData(exec_time_t,task_id_t);
  void processLitmusInterArrivalTimeData(exec_time_t,task_id_t);


public:
  static LitmusDaemon* getInstance();
  void forkDaemon(); 

  void setOverheadObserver(Overhead*);
  void setTaskSetObserver(TaskSet*);

};

#endif
