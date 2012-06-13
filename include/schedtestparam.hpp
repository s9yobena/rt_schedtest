#ifndef SCHED_TEST_PARAM_HPP
#define SCHED_TEST_PARAM_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "schedtrace.h"
#include "rttypes.h"
#include "cmdlparser.h"



using namespace std;

#define START_OVERHEADS 1
#define NBR_OVERHEADS 3
#define START_TASK_PARAM (START_OVERHEADS + NBR_OVERHEADS)

#define MAX_STP_NAME 100

// DO NOT CHANGE THE ORDER OF THE FOLLOWING ENUM
enum ParamPos {
  nbrTaskPos,
  cxsPos,
  schedPos,
  sched2Pos,
  releasePos,
  send_reschedPos,
  release_latencyPos,
  startTaskPos
};

struct TaskParam {
  task_id_t id;
  exec_time_t e;
  exec_time_t d;
  exec_time_t p;
  TaskParam(task_id_t idp, exec_time_t ep, exec_time_t dp, exec_time_t pp)
  {id = idp;e = ep; d = dp; p = pp;}
};

class TaskSet;

class SchedTestParam {

private:
  SchedTestParam();
  SchedTestParam(const SchedTestParam&);
  SchedTestParam& operator=(SchedTestParam&);
  static SchedTestParam *schedTestParamInstance;

  FILE *schedTestPramFile;
  char name[MAX_STP_NAME];

  unsigned nbrTasks;
  TaskSet *taskSet;
  overhead_t cxs;
  overhead_t sched;
  overhead_t sched2;
  overhead_t release;
  overhead_t send_resched;
  overhead_t release_latency;


  
  void setParam(unsigned value, unsigned pos);
  unsigned getParam(unsigned pos);

  void setTaskParam(TaskParam taskParam, unsigned pos);
  void getTaskParam(TaskParam *taskParam, unsigned pos);

  void addAllTasks();
  void getAllTasks();

public:
  static SchedTestParam* getInstance();
  void initSchedTestParam();  
  void resetLocalParams();  

  void setOutputName(const char*);
  char* getOutputName();

  vector<TaskParam> taskParams;

  void setNbrTasks(unsigned);
  void setCXS(overhead_t);
  void setSCHED(overhead_t);
  void setSCHED2(overhead_t);
  void setRELEASE(overhead_t);
  void setSEND_RESCHED(overhead_t);
  void setRELEASE_LATENCY(overhead_t);
  void addTask(TaskParam taskParam);
  void makeSchedTestParam();

  unsigned getNbrTasks();
  overhead_t getCXS();
  overhead_t getSCHED();
  overhead_t getSCHED2();
  overhead_t getRELEASE();
  overhead_t getSEND_RESCHED();
  overhead_t getRELEASE_LATENCY();
  vector<TaskParam> getTaskParams();
  void getSchedTestParam();

  void setParameters(const CmdlParser&);
};

#endif
