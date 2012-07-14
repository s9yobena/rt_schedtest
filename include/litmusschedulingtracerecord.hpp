#ifndef LITMUS_SCHEDULING_TRACE_RECORD_HPP
#define LITMUS_SCHEDULING_TRACE_RECORD_HPP

#include "taskset.hpp" 
#include "schedtrace.h"
#include "rttypes.h"
#include "schedtestparam.hpp"
#include "litmusschedtest.hpp"

class LitmusSchedulingTraceRecord {

protected:

  TaskSet *taskSet;
  ster_t schedulingTraceEventRecordID;    

  SchedTestParam *schedTestParam;
  LitmusSchedTest *litmusSchedTest;

public:

  LitmusSchedulingTraceRecord();
  LitmusSchedulingTraceRecord(ster_t);
  void setTaskSet(TaskSet*);
  ster_t startID;    
  virtual void check(struct st_event_record*)=0;
  virtual void updateTaskSet(lt_t exec_time, pid_t task_id)=0;
};

#endif
