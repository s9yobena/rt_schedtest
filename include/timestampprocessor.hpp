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
  map<pair<cmd_t,uint8_t>,LitmusTimestamp*> registeredLitmusTimestamps;

  bool isRegisteredTimestamp(struct timestamp*);
  void registerTimestamp(struct timestamp*);
  void registerPairLitmusTimestamp(struct timestamp*);
  void registerSingleLitmusTimestamp(struct timestamp*);
  void processRegisteredTimestamp(struct timestamp*);

public:
  static TimestampProcessor* getInstance();
  void processTimestamp(struct timestamp*);
  void setTimestampProcessorObserver(LitmusOverhead*);
  void notifyNewOverhead(overhead_t,cmd_t);
  void setPrintTimestamps(bool);
  void setPrintOverheads(bool);
  
};

#endif
