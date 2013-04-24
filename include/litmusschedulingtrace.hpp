#ifndef LITMUS_SCHEDULING_TRACE_H
#define LITMUS_SCHEDULING_TRACE_H

#include "cmdlparser.h"
#include "schedtrace.h" 	// for struct st_event_record
#include "litmusdevice.hpp"

class SchedulingTraceProcessor;
class TaskSet;

class LitmusSchedulingTrace : public LitmusDevice {

private:

  SchedulingTraceProcessor *schedulingTraceProcessor;
  TaskSet *taskSet;
  
  bool printSchedulingTraces;
  bool printExecutionTimes;

  int eventStrToEventId(const char* eventStr, EventId *eventId);
  void setDefaultConfig();
  void trace();

public:
  LitmusSchedulingTrace(TaskSet*);
  void setParameters(const CmdlParser&);

};

#endif

