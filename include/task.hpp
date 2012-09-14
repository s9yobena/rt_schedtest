#ifndef TASK_HPP
#define TASK_HPP

#include "rttypes.h"
#include "slidingwindow.hpp"

class Task {

private:
  pid_t id;
  struct rt_task parameters;
  lt_t selfSuspension;
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
  double getUtilization();
  lt_t getSelfSuspension();
  lt_t getTardiness();


  void setId(pid_t);
  void setCpu(unsigned);
  void setExecCost(lt_t);
  void setPeriod(lt_t);
  void setSelfSuspension(lt_t);
  void addSelfSuspension(lt_t);
  void setParameters(struct rt_task);

  void updateAvrgExecTime(lt_t execTime);

};

#endif
