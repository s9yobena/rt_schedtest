

#ifndef LITMUS_EXECUTION_TIME_HPP
#define LITMUS_EXECUTION_TIME_HPP

#include "litmusschedulingtracerecord.hpp"
#include "packet2stdout.hpp"


enum LitmusExecutionTimeState {
  
  WAIT_FOR_RELEASE_EVENT,
  WAIT_FOR_COMPLETION_EVENT
};
  

class LitmusExecutionTime : public LitmusSchedulingTraceRecord {

private:

  LitmusExecutionTimeState state;
  struct st_event_record  currentStEventRecord;

  Packet2Stdout *packet2Stdout;

public:

  LitmusExecutionTime(ster_t);
  void check(struct st_event_record*);
  void updateTaskSet(exec_time_t exec_time, task_id_t task_id);
};

#endif
