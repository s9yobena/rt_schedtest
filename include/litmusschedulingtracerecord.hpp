#ifndef LITMUS_SCHEDULING_TRACE_RECORD_HPP
#define LITMUS_SCHEDULING_TRACE_RECORD_HPP

#include "packet2stdout.hpp"

class LitmusSchedulingTraceRecord {

protected:

  Packet2Stdout *packet2Stdout;
  ster_t schedulingTraceEventRecordID;    

public:

  LitmusSchedulingTraceRecord();
  LitmusSchedulingTraceRecord(ster_t);
  ster_t startID;    
  virtual void check(struct st_event_record*)=0;
  virtual void updateTaskSet(exec_time_t exec_time, task_id_t task_id)=0;
};

#endif
