#include "litmusdevice.hpp"

// This queue holds all the devices for scheduling
queue<LitmusDevice*> LitmusDevice::devQueue;

LitmusDevice::LitmusDevice() {

  devQueue.push(this);
}

int LitmusDevice::initDev(const char* devName) {
  
  this->devName = devName;
  devFD = open(devName, O_RDWR);
  if (devFD < 0) {
    perror("could not open feathertrace");
    return 1;
  }
  nbrEvents = 0;
  setDefaultConfig();
}

void LitmusDevice::startTracing() {
  trace();
}

// Performs a round-robin scheduling among all the devices
void LitmusDevice::scheduleTrace() {
  
  // put the current device at the end of the queue
  devQueue.push(devQueue.front());
  
  // remove the current device from the front of the queue
  devQueue.pop();

  // schedules a trace on the oldest device that run trace()
  cout<<"scheduling device: "<<devQueue.front()->devName<<endl;
  devQueue.front()->trace();
}

void LitmusDevice::stopTracing() {
  int ok;
  ok = disableAllEvents();
  if (!ok)
    fprintf(stderr, "disable_all: %m\n");
}

int LitmusDevice::enableEvent(char* eventStr) {
  EventId *eventId;
  int err;
  eventId = devEvents + nbrEvents;
  if (eventStrToEventId(eventStr, eventId)) {
    
    perror("could not convert event name to event id");
    return 1;
  }
  nbrEvents +=1;
  err = ioctl(devFD, ENABLE_CMD, *eventId);
  
  if (err < 0) {
    printf("ioctl(%d, %d, %d) => %d (errno: %d)\n", devFD, (int) ENABLE_CMD, *eventId, err, errno);

    return err != 0;
  }
}

int LitmusDevice::disableAllEvents() {
  int disabled = 0;
  fprintf(stderr, "Disabling %u events.\n", nbrEvents);
  for (int i = 0; i < nbrEvents; i++){
    if (ioctl(devFD, DISABLE_CMD, devEvents[i]) < 0)
      perror("ioctl(DISABLE_CMD)");
    else{
      disabled++;
    }
  }
  return  disabled == this->nbrEvents;
}
