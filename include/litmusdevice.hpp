#ifndef LITMUS_DEVICE_HPP
#define LITMUS_DEVICE_HPP

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>
#include <sys/ioctl.h>
#include<queue>
#include<iostream>
using namespace std;

#define ENABLE_CMD  0L
#define DISABLE_CMD 1L

#define DEV_BUF_SIZE 4012
#define MAX_EVENTS 128 

typedef unsigned int EventId;

class LitmusDevice {

protected:

  LitmusDevice(const LitmusDevice&);
  LitmusDevice & operator=(const LitmusDevice&);
 
  const char* devName;
  int devFD;
  char devBuffer[DEV_BUF_SIZE];
  EventId devEvents[MAX_EVENTS];
  unsigned int nbrEvents;

  static queue<LitmusDevice*> devQueue;
  
  int enableEvent(char* eventStr);
  // provide implementation when subclassing
  virtual int eventStrToEventId(const char* eventStr, EventId *eventId)=0;
  int disableAllEvents();
  virtual void trace()=0;
  
public:
  
  LitmusDevice();
  int initDev(const char* devName);
  virtual void setDefaultConfig()=0;
  void startTracing();
  void stopTracing();
  void scheduleTrace();
};

#endif
