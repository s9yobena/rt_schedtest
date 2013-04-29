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
#include<list>
#include<iostream>
#include <pthread.h>
#include <assert.h>
using namespace std;

#define ENABLE_CMD  0L
#define DISABLE_CMD 1L

// #define NO_EVENTS (1<<13)
#define NO_EVENTS 100
#define DEV_BUF_SIZE (25*NO_EVENTS)
#define MAX_EVENTS 128 

enum buf_status_t {full,empty};
enum buf_work_t {busy,idle};

struct dev_buf_t {
  char devName[100];
  int devFD;
  char devBuffer[DEV_BUF_SIZE];
  size_t size;
  enum buf_status_t status;
  enum buf_work_t work;
  pthread_cond_t full;
  pthread_cond_t empty;
  pthread_mutex_t mutex;
};

typedef unsigned int EventId;

class CmdlParser;
class LitmusDevice {

protected:

  LitmusDevice(const LitmusDevice&);
  LitmusDevice & operator=(const LitmusDevice&);
 
  pthread_t asynch_reader;
  struct dev_buf_t dev_buf;
  
  EventId devEvents[MAX_EVENTS];
  unsigned int nbrEvents;

  static list<LitmusDevice*> devList;
  static bool doTrace;
  
  int enableEvent(const char* eventStr);
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

  virtual void setParameters(const CmdlParser&)=0;

  static void startTracingAllDevices();
  static void stopAllDevices();

};

#endif
