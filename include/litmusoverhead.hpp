

#ifndef LITMUS_OVERHEAD_H
#define LITMUS_OVERHEAD_H

#include "timestampprocessor.hpp"
#include "cmdlparser.h"
#include "litmusdevice.hpp"
#include "timestamp.h"
#include "packet2stdout.hpp"

typedef unsigned long long overhead_t;

class TimestampProcessor;

class LitmusOverhead : public LitmusDevice {

private:

  LitmusOverhead();
  LitmusOverhead(const LitmusOverhead&);
  LitmusOverhead & operator=(const LitmusOverhead&);
  static LitmusOverhead* litmusOverheadInstance;
  TimestampProcessor *timestampProcessor;
  Packet2Stdout *packet2Stdout;

  bool printTimestamps;
  bool printOverheads;

  overhead_t maxCXS;
  overhead_t maxSCHED;
  overhead_t maxSCHED2;
  overhead_t maxRELEASE;
  overhead_t maxRELEASE_LATENCY;
  overhead_t maxSEND_RESCHED;

  int eventStrToEventId(const char* eventStr, EventId *eventId);
  void setDefaultConfig();
  void trace();

public:
  
  static LitmusOverhead* getInstance();
  void setParameters(const CmdlParser&);
  void checkMaxCXS(overhead_t);
  void checkMaxSCHED(overhead_t);
  void checkMaxSCHED2(overhead_t);
  void checkMaxRELEASE(overhead_t);
  void checkMaxSEND_RESCHED(overhead_t);
  void checkMaxRELEASE_LATENCY(overhead_t);

};

#endif
