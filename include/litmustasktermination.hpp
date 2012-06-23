#ifndef LITMUS_TASK_TERMINATION_HPP
#define LITMUS_TASK_TERMINATION_HPP

#include "litmusschedulingtracerecord.hpp"
#include "schedtestparam.hpp"
#include "litmusschedtest.hpp"


class LitmusTaskTermination : public LitmusSchedulingTraceRecord {

private:
  SchedTestParam *schedTestParam;
  LitmusSchedTest *litmusSchedTest;

public:

  LitmusTaskTermination(ster_t);
  void check(struct st_event_record*);
  void updateTaskSet(lt_t termination, pid_t task_id);
};

#endif
