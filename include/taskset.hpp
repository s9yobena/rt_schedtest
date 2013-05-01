#ifndef TASKSET_H
#define TASKSET_H

#include <cstdio>
#include "rttypes.h"
#include "litmus.h"
#include "task.hpp"
#include "cmdlparser.h"
#include <map>
#include <vector>
#include <unordered_map>
#include "schedtrace.h"
#include "schedtestparam.hpp"

class TaskSet {

private:

  TaskSet(const TaskSet&);
  TaskSet& operator=(const TaskSet&);
  bool printExecutionTimes;
  bool printDebug;
  
  lt_t averageExecCost;
  
  bool isNewTask(pid_t task_id);
  void addTask(pid_t task_id);
  bool updateMaxExecCost(lt_t exec_time, pid_t task_id); // exec_time in NS
  void updateMinInterArrivalTime(lt_t exec_time, pid_t task_id);  
  void updateSumSelfSuspension(lt_t self_suspension_time, pid_t task_id);
  void updatePerJobMaxSelfSusp(lt_t self_suspension_time, pid_t task_id);

public :  

  unordered_map<pid_t,Task> tasks;

  TaskSet();  
  void updateAllTasks(SchedTestParam*);
  bool updateTaskExecCost(lt_t exec_time, pid_t task_id);
  void updateTaskInterArrivalTime(lt_t inter_arrival_time, pid_t task_id);
  void updateTaskSelfSuspension(lt_t self_suspension_time, pid_t task_id);
  void setParameters(const CmdlParser&);
  void printParameters();

  int getNbrTasks();
  unsigned getTaskCpu(pid_t);
  lt_t getTaskExecCost(pid_t);
  lt_t getTaskAvrgExecCost(pid_t);
  lt_t getTaskDeadline(pid_t);
  lt_t getTaskPeriod(pid_t);
  double getTaskUtilization(pid_t);
  lt_t getTaskSelfSuspension(pid_t);
  lt_t getPerJobMaxSelfSusp(pid_t);
  lt_t getTaskTardiness(pid_t);
  
  void setTaskCpu(pid_t, unsigned);
  void setTaskExecCost(pid_t, lt_t);
  void setTaskPeriod(pid_t, lt_t);
  void setTaskSelfSuspension(pid_t, lt_t);
  
  void addTask(Task*);
  void removeTask(pid_t);
  Task* getTask(pid_t);

  lt_t computeAverageExecCost();
  lt_t getAverageExecCost();
  void setAverageExecCost(lt_t);

};

#endif
