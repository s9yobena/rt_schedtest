#ifndef TASKSET_H
#define TASKSET_H

#include <cstdio>

#include "litmus.h"

#include "cmdlparser.h"

#include "schedtrace.h"

#define MAX_RT_TASK_NR 100

class TaskSet {

private:
  TaskSet();
  TaskSet(const TaskSet&);
  TaskSet& operator=(const TaskSet&);
  static TaskSet* taskSetInstance;
  bool printExecutionTimes;

  bool isNewTask(task_id_t task_id);
  void addTask(task_id_t task_id);
  void updateMaxExecCost(exec_time_t exec_time, task_id_t task_id); // exec_time in NS
  void updateMinInterArrivalTime(exec_time_t exec_time, task_id_t task_id);  

public :  
  int  rt_task_id[MAX_RT_TASK_NR];
  struct rt_task rt_task_param[MAX_RT_TASK_NR]; // times in NS
  int nb_rts;
  
  static TaskSet* getInstance();
  void updateTaskExecCost(exec_time_t exec_time, task_id_t task_id);
  void updateTaskInterArrivalTime(exec_time_t inter_arrival_time, task_id_t task_id);
  void setParameters(const CmdlParser&);
  void printParameters();

};

#endif
