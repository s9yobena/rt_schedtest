

#ifndef LITMUS_EXECUTION_TIME_HPP
#define LITMUS_EXECUTION_TIME_HPP

#include <iostream>
#include "schedtrace.h"
#include "litmusschedulingtracerecord.hpp"

using namespace std;


enum LitmusExecutionTimeState {
  
  WAIT_FOR_RELEASE_EVENT,
  WAIT_FOR_COMPLETION_EVENT
};
  

class LitmusExecutionTime : public LitmusSchedulingTraceRecord {

private:

  LitmusExecutionTimeState state;
  struct st_event_record  currentStEventRecord;
public:

  LitmusExecutionTime(ster_t);
  void check(struct st_event_record*);
};

#endif
