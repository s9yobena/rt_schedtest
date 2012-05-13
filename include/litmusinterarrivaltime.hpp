#ifndef LITMUS_INTERARRIVAL_TIME_HPP
#define LITMUS_INTERARRIVAL_TIME_HPP

#include "litmusschedulingtracerecord.hpp"
#include "packet2stdout.hpp"

enum LitmusInterArrivalTimeState {
  
  IAT_WAIT_FOR_COMPLETION_EVENT,
  IAT_WAIT_FOR_RELEASE_EVENT
};
  
class LitmusInterArrivalTime : public LitmusSchedulingTraceRecord {

private:

  LitmusInterArrivalTimeState state;
  struct st_event_record  currentStEventRecord;

  Packet2Stdout *packet2Stdout;

public:

  LitmusInterArrivalTime(ster_t);
  void check(struct st_event_record*);
  void updateTaskSet(exec_time_t inter_arrival_time, task_id_t task_id);
};

#endif
