#ifndef SCHEDULING_TRACE_PROCESSOR_H
#define SCHEDULING_TRACE_PROCESSOR_H

#include "schedtrace.h"
#include "litmusexecutiontime.hpp"
// #include "litmusschedulingtrace.hpp"
#include "taskset.hpp"
#include <inttypes.h>
#include <iostream>
#include <map>
using namespace std;


// class LitmusSchedulingTrace;
class LitmusSchedulingTraceRecord;

class SchedulingTraceProcessor {

private:

  SchedulingTraceProcessor(const SchedulingTraceProcessor&);
  SchedulingTraceProcessor & operator=(const SchedulingTraceProcessor&);

  // LitmusSchedulingTrace* litmusSchedulingTrace;
  TaskSet *taskSet;
  bool printSchedulingTraces;
  bool printExecutionTimes;
  map<pair<int,int>,LitmusSchedulingTraceRecord*> registeredTraceRecords;

  bool isRegisteredSchedulingTrace(struct st_event_record*);
  bool registerSchedulingTrace(struct st_event_record*);
  void processRegisteredSchedulingTrace(struct st_event_record*);

public:
  
  SchedulingTraceProcessor();
  void processSchedulingTrace(struct st_event_record*);
  // void setSchedulingTraceProcessorObserver(LitmusSchedulingTrace*);
  void setSchedulingTraceProcessorObserver(TaskSet*);
  void notifyNewExecutionTime(exec_time_t, task_id_t);
  void setPrintSchedulingTraces(bool);
  void setPrintExecutionTimes(bool);
  
};

#endif
