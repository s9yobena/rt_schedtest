

#ifndef LITMUS_EXECUTION_TIME_HPP
#define LITMUS_EXECUTION_TIME_HPP

#include "litmusschedulingtracerecord.hpp"

enum LitmusExecutionTimeState {
  
  WAIT_FOR_RELEASE_EVENT,
  WAIT_FOR_COMPLETION_EVENT,
  TASK_TERMINATED
};
  

class LitmusExecutionTime : public LitmusSchedulingTraceRecord {

private:

  LitmusExecutionTimeState state;
  struct st_event_record  currentStEventRecord;

  unsigned long long lastJobNo;

public:

  LitmusExecutionTime(ster_t);
  void check(struct st_event_record*);
  void updateTaskSet(lt_t exec_time, unsigned cpu, pid_t task_id);

  void setLastJobNo(unsigned long long);
  unsigned long long getLastJobNo();

};

#endif
