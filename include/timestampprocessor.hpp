#ifndef TIMESTAMP_PROCESSOR_H
#define TIMESTAMP_PROCESSOR_H

#include "timestamp.h"
#include "litmusoverhead.hpp"
#include "singlelitmustimestamp.hpp"
#include "pairlitmustimestamp.hpp"
#include "litmusoverhead.hpp"
#include <iostream>
#include <map>
#include <set>
using namespace std;

typedef unsigned long long overhead_t;

class LitmusOverhead;
class LitmusTimestamp;

class TimestampProcessor {

private:
  TimestampProcessor();
  TimestampProcessor(const TimestampProcessor&);
  TimestampProcessor & operator=(const TimestampProcessor&);
  static TimestampProcessor* timestampProcessorInstance;
  LitmusOverhead* litmusOverhead;
  bool printTimestamps;
  bool printOverheads;
  set<cmd_t> registeredEvents;
  map<cmd_t,LitmusTimestamp*> registeredLitmusTimestamps;

public:
  static TimestampProcessor* getInstance();
  void processTimestamp(struct timestamp*);
  void setTimestampProcessorObserver(LitmusOverhead*);
  void registerEvent(char*);
  void notifyNewOverhead(overhead_t,cmd_t);
  void setPrintTimestamps(bool);
  void setPrintOverheads(bool);
  
};

#endif
