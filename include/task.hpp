#ifndef TASK_HPP
#define TASK_HPP

#include "rttypes.h"
#include "slidingwindow.hpp"

class Task {

private:
  pid_t id;
  struct rt_task parameters;
  lt_t selfSuspension;
  lt_t perJobMaxSelfSusp;
  SlidingWindow<lt_t> execTimeSW;	// used to compute task average execution time over a sliding window

public:
  Task();
  ~Task();
  pid_t getId();
  unsigned getCpu();
  lt_t getExecCost();
  lt_t getAvrgExecTime();
  lt_t getDeadline();
  lt_t getPeriod(); 
  lt_t getSelfSuspension(); 	// This is how long the task suspended, i.e the sum of all self suspensions
  lt_t getPerJobMaxSelfSusp(); 	// This is the maximum time a job suspended.
  double getUtilization();
  lt_t getTardiness();

  void setId(pid_t);
  void setCpu(unsigned);
  void setExecCost(lt_t);
  void setPeriod(lt_t);
  void setSelfSuspension(lt_t);
  void addSelfSuspension(lt_t);
  void setPerJobMaxSelfSusp(lt_t);
  void setParameters(struct rt_task);

  void updateAvrgExecTime(lt_t execTime);

};

#endif
