#ifndef TASKSET_H
#define TASKSET_H

#include <cstdio>
#include "rttypes.h"
#include "litmus.h"
#include "task.hpp"
#include "cmdlparser.h"
#include <map>
#include <vector>
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
  
  vector<pid_t> tasksId;
  map<pid_t,Task> taskSet;
  
  bool isNewTask(pid_t task_id);
  void addTask(pid_t task_id);
  void updateMaxExecCost(lt_t exec_time, pid_t task_id); // exec_time in NS
  void updateMinInterArrivalTime(lt_t exec_time, pid_t task_id);  
  void updateSumSelfSuspension(lt_t self_suspension_time, pid_t task_id);
  
public :  
  
  static TaskSet* getInstance();
  void updateAllTasks(SchedTestParam*);
  void updateTaskExecCost(lt_t exec_time, pid_t task_id);
  void updateTaskInterArrivalTime(lt_t inter_arrival_time, pid_t task_id);
  void updateTaskSelfSuspension(lt_t self_suspension_time, pid_t task_id);
  void setParameters(const CmdlParser&);
  void printParameters();

  int getNbrTasks();
  pid_t getTaskId(int);
  lt_t getTaskExecCost(pid_t);
  lt_t getTaskPeriod(pid_t);
  lt_t getTaskSelfSuspension(pid_t);

  void removeTask(pid_t);

};

#endif
