#ifndef LITMUS_OVERHEAD_H
#define LITMUS_OVERHEAD_H

#include "overhead.hpp"
#include "timestampprocessor.hpp"
#include "cmdlparser.h"
#include "litmusdevice.hpp"
// #include <cstdio>
// #include <cstdlib>
// #include <fcntl.h>
// #include <signal.h>
// #include <errno.h>
// #include <cstring>
// #include <unistd.h>
// #include <sys/ioctl.h>
#include "timestamp.h"
// #include <arpa/inet.h>
// #include "mapping.h"

// #define MAX_EVENTS 128
// #define CYCLES_PER_US 2128
// #define TRACE_BUF_SIZE 500000

typedef unsigned long long overhead_t;

class TimestampProcessor;

class LitmusOverhead : public LitmusDevice {

private:

  LitmusOverhead();
  LitmusOverhead(const LitmusOverhead&);
  LitmusOverhead & operator=(const LitmusOverhead&);
  static LitmusOverhead* litmusOverheadInstance;


  Overhead* overhead;
  TimestampProcessor *timestampProcessor;

  // const char* traceBufferName;
  // char *debug;
  // int traceBufFD;
  // char traceBuffer[TRACE_BUF_SIZE];
  // unsigned long bytesRead;
  // char* traceEvent[MAX_EVENTS];
  // int eventCount;

  // cmd_t  ids[MAX_EVENTS];
  // int nbTraceEvents;

  bool printTimestamps;
  bool printOverheads;

  overhead_t maxCXS;
  overhead_t maxSCHED;
  overhead_t maxSCHED2;
  overhead_t maxRELEASE;
  overhead_t maxRELEASE_LATENCY;
  overhead_t maxSEND_RESCHED;
  
  // LitmusTimestamp cxsLT;
  // LitmusTimestamp schedLT;
  // LitmusTimestamp sched2LT;
  // LitmusTimestamp releaseLT;
  // LitmusTimestamp release_latencyLT;
  // LitmusTimestamp send_reschedLT;


  unsigned long long threshold; /* 10 ms == 10 full ticks */
  int wantBestEffort;
  int wantInterleaved;
  // void addEvent(char*);
  // void startTracing(int);

  void updateMaxOverhead(struct timestamp* start, struct timestamp* end, 
			   unsigned long id);
  void updateMaxOverhead2(struct timestamp* start, struct timestamp* end, 
			   unsigned long id);
  
  struct timestamp* next(struct timestamp* start, struct timestamp* end,
			 int cpu);
  struct timestamp* next_id(struct timestamp* start, struct timestamp* end,
			    int cpu, unsigned long id,
			    unsigned long stop_id);
  // int enableEvent(int,char*);
  // int disableAll(int fd);
  int eventStrToEventId(const char* eventStr, EventId *eventId);


  void setDefaultConfig();
  void trace();
  void updateLitmusOverheadObservers();

public:
  
  static LitmusOverhead* getInstance();
  // void startTracing();
  void setParameters(const CmdlParser&);
  // int initOverhead(const char*);
  // void stopTracing();
  void setLitmusOverheadObserver(Overhead*);
  void checkMaxCXS(overhead_t);
  void checkMaxSCHED(overhead_t);
  void checkMaxSCHED2(overhead_t);
  void checkMaxRELEASE(overhead_t);
  void checkMaxSEND_RESCHED(overhead_t);
  void checkMaxRELEASE_LATENCY(overhead_t);

};

#endif
