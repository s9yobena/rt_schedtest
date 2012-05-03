#include "timestamp.h"
#include "timestampprocessor.hpp"


#ifndef LITMUS_TIMESTAMP_HPP
#define LITMUS_TIMESTAMP_HPP

enum LitmusTimestampState {
  
  WAIT_FOR_START_EVENT,
  WAIT_FOR_MATCH
};


typedef unsigned long long overhead_t;
class TimestampProcessor;

class LitmusTimestamp {

private:
  TimestampProcessor* timestampProcessor;
public:
  LitmusTimestamp();
  LitmusTimestamp(cmd_t);
  cmd_t startID;    
  virtual void check(struct timestamp*)=0;
  void setLitmusTimestampObserver(TimestampProcessor*);
  void updateLitmusTimestampObservers(overhead_t,cmd_t);
};

#endif
