#ifndef LITMUS_INTERARRIVAL_TIME_HPP
#define LITMUS_INTERARRIVAL_TIME_HPP

#include <iostream>
#include "schedtrace.h"
#include "litmusschedulingtracerecord.hpp"

using namespace std;


enum LitmusInterArrivalTimeState {
  
  WAIT_FOR_COMPLETION_EVENT,
  WAIT_FOR_RELEASE_EVENT
};
  
class LitmusInterArrivalTime : public LitmusSchedulingTraceRecord {

private:

  LitmusInterArrivalTimeState state;
  struct st_event_record  currentStEventRecord;

public:

  LitmusInterArrivalTime(ster_t);
  void check(struct st_event_record*);
};

#endif
