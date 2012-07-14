
#ifndef SCHEDULING_TRACE_PROCESSOR_H
#define SCHEDULING_TRACE_PROCESSOR_H

#include "schedtrace.h"
#include "litmusexecutiontime.hpp"
#include "litmusinterarrivaltime.hpp"
#include "litmusselfsuspension.hpp"
#include "litmustasktermination.hpp"
#include <inttypes.h>
#include <iostream>
#include <map>
using namespace std;

class LitmusSchedulingTraceRecord;
class TaskSet;

class SchedulingTraceProcessor {

private:

  SchedulingTraceProcessor(const SchedulingTraceProcessor&);
  SchedulingTraceProcessor & operator=(const SchedulingTraceProcessor&);

  TaskSet *taskSet;

  bool printSchedulingTraces;
  bool printExecutionTimes;
  map<pair<int,int>,LitmusSchedulingTraceRecord*> registeredTraceRecords;
  map<pair<int,int>,LitmusSchedulingTraceRecord*> registeredInterArrivalTimeTraceRocords;  
  map<pair<int,int>,LitmusSchedulingTraceRecord*> registeredSelfSuspensions;  

  bool isRegisteredSchedulingTrace(struct st_event_record*);
  bool registerSchedulingTrace(struct st_event_record*);
  void registerLitmusExecutionTime(struct st_event_record* ster);
  void registerLitmusInterArrivalTime(struct st_event_record* ster);
  void registerLitmusSelfSuspension(struct st_event_record* ster);
  void processRegisteredSchedulingTrace(struct st_event_record*);

public:
  
  SchedulingTraceProcessor();
  void processSchedulingTrace(struct st_event_record*);
  void setTaskSet(TaskSet *);

  void setPrintSchedulingTraces(bool);
  void setPrintExecutionTimes(bool);
  
};

#endif
