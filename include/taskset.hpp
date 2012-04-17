#ifndef TASKSET_H
#define TASKSET_H

#include <cstdio>


#include "litmus.h"

#include "cmdlparser.h"

#define MAX_RT_TASK_NR 10


class TaskSet {

private:
  TaskSet();
  TaskSet(const TaskSet&);
  TaskSet& operator=(const TaskSet&);
  static TaskSet* taskSetInstance;
  


public :  
  int  rt_task_id[MAX_RT_TASK_NR];
  struct rt_task rt_task_param[MAX_RT_TASK_NR];
  int nb_rts;
  
  static TaskSet* getInstance();
  void setParameters(const CmdlParser&);
  void printParameters();

};

#endif
