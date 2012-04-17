#ifndef LITMUS_OVERHEAD_H
#define LITMUS_OVERHEAD_H

#include "overhead.hpp"


#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include "timestamp.h"
// #include <arpa/inet.h>
// #include "mapping.h"

#define MAX_EVENTS 128
#define CYCLES_PER_US 2128
#define TRACE_BUF_SIZE 4096

typedef unsigned long long overhead_t;

class LitmusOverhead {

private:

  LitmusOverhead();
  LitmusOverhead(const LitmusOverhead&);
  LitmusOverhead & operator=(const LitmusOverhead&);
  static LitmusOverhead* litmusOverheadInstance;


  Overhead* overhead;

  const char* traceBufferName;
  char *debug;
  int traceBufFD;
  char traceBuffer[TRACE_BUF_SIZE];
  unsigned long bytesRead;
  char* traceEvent[MAX_EVENTS];
  int eventCount;

  cmd_t  ids[MAX_EVENTS];
  int nbTraceEvents;

  overhead_t maxCXS;
  overhead_t maxSCHED;
  overhead_t maxSCHED2;
  overhead_t maxRELEASE;
  overhead_t maxRELEASE_LATENCY;
  overhead_t maxSEND_RESCHED;

  unsigned long long threshold; /* 10 ms == 10 full ticks */
  int wantBestEffort;
  int wantInterleaved;
  void addEvent(char*);
  void startTracing(int);
  void updateMaxOverhead(struct timestamp* start, struct timestamp* end, 
			   unsigned long id);
  void updateMaxOverhead2(struct timestamp* start, struct timestamp* end, 
			   unsigned long id);
  
  struct timestamp* next(struct timestamp* start, struct timestamp* end,
			 int cpu);
  struct timestamp* next_id(struct timestamp* start, struct timestamp* end,
			    int cpu, unsigned long id,
			    unsigned long stop_id);
  int enableEvent(int,char*);
  int disableAll(int fd);

  void updateLitmusOverheadObservers();

public:
  
  static LitmusOverhead* getInstance();
  int initOverhead(const char*);
  void stopTracing();
  void setLitmusOverheadObserver(Overhead*);


};

#endif
