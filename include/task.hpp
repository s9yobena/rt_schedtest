#ifndef TASK_HPP
#define TASK_HPP

#include "rttypes.h"

class Task {

private:
  pid_t id;
  struct rt_task parameters;
  lt_t selfSuspension;

public:
  Task();
  ~Task();
  unsigned getCpu();
  lt_t getExecCost();
  lt_t getPeriod(); 
  lt_t getSelfSuspension();

  void setExecCost(lt_t);
  void setPeriod(lt_t);
  void setSelfSuspension(lt_t);
  void addSelfSuspension(lt_t);
  void setParameters(struct rt_task);

};

#endif
