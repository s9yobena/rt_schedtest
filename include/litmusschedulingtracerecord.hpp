#include "schedtrace.h"
#include "schedulingtraceprocessor.hpp"

#ifndef LITMUS_SCHEDULING_TRACE_RECORD_HPP
#define LITMUS_SCHEDULING_TRACE_RECORD_HPP


class SchedulingTraceProcessor;

class LitmusSchedulingTraceRecord {

private:

  SchedulingTraceProcessor* schedulingTraceProcessor;
  ster_t schedulingTraceEventRecordID;    

public:

  LitmusSchedulingTraceRecord();
  LitmusSchedulingTraceRecord(ster_t);
  ster_t startID;    
  virtual void check(struct st_event_record*)=0;
  void setLitmusSchedulingTraceRecordObserver(SchedulingTraceProcessor*);
  void updateLitmusSchedulingTraceRecordObservers(exec_time_t, task_id_t);

};

#endif
