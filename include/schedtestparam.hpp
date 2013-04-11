#ifndef SCHED_TEST_PARAM_HPP
#define SCHED_TEST_PARAM_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "schedtrace.h"
#include "rttypes.h"
#include "cmdlparser.h"
#include <sstream>


using namespace std;

// DO NOT CHANGE THE ORDER OF THE FOLLOWING ENUM
enum ParamPos {
  cpuClockPos,
  cacheTopPos,
  cxsPos,
  schedPos,
  sched2Pos,
  releasePos,
  send_reschedPos,
  release_latencyPos,
  tickPos,
  startTaskPos
};

struct TaskParam {
  pid_t id;
  unsigned cpu;
  lt_t e;
  lt_t d;
  lt_t p;
  lt_t ss;			// sum self suspension
  lt_t perJobMaxSelfSusp;
  TaskParam(pid_t idp, unsigned _cpu, lt_t ep, lt_t dp, lt_t pp, lt_t pss, lt_t _perJobMaxSelfSusp)
  {id = idp; cpu = _cpu; e = ep; d = dp; p = pp; ss = pss; perJobMaxSelfSusp = _perJobMaxSelfSusp;}
};

class TaskSet;
class CacheTop;

class SchedTestParam {

private:
  SchedTestParam();
  SchedTestParam(const SchedTestParam&);
  SchedTestParam& operator=(SchedTestParam&);
  static SchedTestParam *schedTestParamInstance;

  FILE *schedTestPramFile;
  char name[maxSTFname];
  stringstream ssname;
  int suffI;			// suffix for name (intger)
  long maxSTFiles;		// max number of active (i.e., concurrently used)
			        //   stf files

  unsigned mhzCpuClock;
  vector<vector<int> > cache_top;
  overhead_t cxs;
  overhead_t sched;
  overhead_t sched2;
  overhead_t release;
  overhead_t send_resched;
  overhead_t release_latency;
  overhead_t tick;
  TaskSet *taskSet;

  CacheTop *cacheTop;
  vector<vector<int> > _cacheTop;
  void setCacheTopParam(unsigned cacheTopPos);
  vector<vector<int> > getCacheTopParam(unsigned cacheTopPos);
  char cacheTopBuf[100];

  
  void setParam(unsigned value, unsigned pos);
  unsigned getParam(unsigned pos);

  void setTaskParam(TaskParam taskParam, unsigned pos);
  int getTaskParam(TaskParam *taskParam, unsigned pos);

  void addAllTasks();
  void getAllTasks();
  
  const char* endSchedTestParam;
  int endOfSchedTestParam(const char*);
  void addEndMark();

  void deleteSTFile();

public:
  static SchedTestParam* getInstance();
  void initSchedTestParam();  
  void initCacheTopParam(); 
  void resetLocalParams();  
  
  void initOutputName();
  void setOutputName(const char*);
  char* getOutputName();
  void generateOutputName();
  void setTaskSet(TaskSet*);

  vector<TaskParam> taskParams;

  void setMHzCpuClock(unsigned);
  void setCXS(overhead_t);
  void setSCHED(overhead_t);
  void setSCHED2(overhead_t);
  void setRELEASE(overhead_t);
  void setSEND_RESCHED(overhead_t);
  void setRELEASE_LATENCY(overhead_t);
  void setTICK(overhead_t);
  void addTask(TaskParam taskParam);
  void makeSchedTestParam();
  void writeSchedTestParams();
  
  unsigned getNbrCpus();
  unsigned getMHzCpuClock();  
  vector<vector<int> > getCacheTop();
  overhead_t getCXS();
  overhead_t getSCHED();
  overhead_t getSCHED2();
  overhead_t getRELEASE();
  overhead_t getSEND_RESCHED();
  overhead_t getRELEASE_LATENCY();
  overhead_t getTICK();
  vector<TaskParam> getTaskParams();
  void getSchedTestParam();

  void setParameters(const CmdlParser&);
  
};

#endif
