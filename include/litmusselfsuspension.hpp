#ifndef LITMUS_SELF_SUSPENSION_HPP
#define LITMUS_SELF_SUSPENSION_HPP

#include "litmusschedulingtracerecord.hpp"

enum LitmusSelfSuspensionState {
  
  WAIT_FOR_BLOCK_EVENT,
  WAIT_FOR_RESUME_EVENT
};
  

class LitmusSelfSuspension : public LitmusSchedulingTraceRecord {

private:

  LitmusSelfSuspensionState state;
  struct st_event_record  currentStEventRecord;

public:

  LitmusSelfSuspension(ster_t);
  void check(struct st_event_record*);
  void updateTaskSet(lt_t self_suspension_time, pid_t task_id);
};

#endif
