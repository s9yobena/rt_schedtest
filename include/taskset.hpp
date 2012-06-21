#ifndef TASKSET_H
#define TASKSET_H

#include <cstdio>
#include "rttypes.h"
#include "litmus.h"

#include "cmdlparser.h"

#include "schedtrace.h"
#include "schedtestparam.hpp"

#define MAX_RT_TASK_NR 100

class TaskSet {

private:
  TaskSet();
  TaskSet(const TaskSet&);
  TaskSet& operator=(const TaskSet&);
  static TaskSet* taskSetInstance;
  bool printExecutionTimes;

  bool isNewTask(pid_t task_id);
  void addTask(pid_t task_id);
  void updateMaxExecCost(lt_t exec_time, pid_t task_id); // exec_time in NS
  void updateMinInterArrivalTime(lt_t exec_time, pid_t task_id);  
  void updateSumSelfSuspension(lt_t self_suspension_time, pid_t task_id);

public :  
  int  rt_task_id[MAX_RT_TASK_NR];
  struct rt_task rt_task_param[MAX_RT_TASK_NR]; // times in NS
  lt_t rt_task_SelfSuspension[MAX_RT_TASK_NR]; // times in NS
  int nb_rts;
  
  static TaskSet* getInstance();
  void updateAllTasks(SchedTestParam*);
  void updateTaskExecCost(lt_t exec_time, pid_t task_id);
  void updateTaskInterArrivalTime(lt_t inter_arrival_time, pid_t task_id);
  void updateTaskSelfSuspension(lt_t self_suspension_time, pid_t task_id);
  void setParameters(const CmdlParser&);
  void printParameters();

};

#endif
