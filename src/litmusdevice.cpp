#include "litmusdevice.hpp"

// This queue holds all the devices for scheduling
queue<LitmusDevice*> LitmusDevice::devQueue;

LitmusDevice::LitmusDevice() {

  devQueue.push(this);
}

void *dev_reader_func(void *_arg) {
  int rd;
  struct dev_buf_t *dev_buf;
  dev_buf = (struct dev_buf_t*)_arg;

  while (1) {
	
    pthread_mutex_lock(&dev_buf->mutex);
	
    while (dev_buf->status == full) {
      // Wait for the daemon to read data from buffer.
      pthread_cond_wait(&dev_buf->empty, &dev_buf->mutex);
    }
    
    // If this call blocks, which is the case if there is no data available in 
    //the device, the daemon is still able to read other devices.
    if ((rd = read(dev_buf->devFD, dev_buf->devBuffer, DEV_BUF_SIZE)) > 0) {
      dev_buf->status = full;
      dev_buf->size = rd;
    }
    
    // Notice that there is no pthread_cond_signal(&dev_buf->full), because the 
    //daemon does not block if there is no data read from this device.
    pthread_mutex_unlock(&dev_buf->mutex);
  }


}

int LitmusDevice::initDev(const char* devName) {

  dev_buf.status = empty;
  dev_buf.size = 0;
  assert(pthread_mutex_init(&dev_buf.mutex, 0)==0);
  assert(pthread_cond_init(&dev_buf.full, 0)==0);
  assert(pthread_cond_init(&dev_buf.empty, 0)==0);
	
  strcpy(this->dev_buf.devName, devName);
  dev_buf.devFD = open(dev_buf.devName, O_RDWR);
  if (dev_buf.devFD < 0) {
    perror("could not open feathertrace");
    return 1;
  }

  nbrEvents = 0;
  setDefaultConfig();

  pthread_create(&asynch_reader, NULL, dev_reader_func, (void*) &dev_buf);
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
  err = ioctl(dev_buf.devFD, ENABLE_CMD, *eventId);
  
  if (err < 0) {
    printf("ioctl(%d, %d, %d) => %d (errno: %d)\n", dev_buf.devFD, (int) ENABLE_CMD, *eventId, err, errno);

    return err != 0;
  }
}

int LitmusDevice::disableAllEvents() {
  int disabled = 0;
  fprintf(stderr, "Disabling %u events.\n", nbrEvents);
  for (int i = 0; i < nbrEvents; i++){
    if (ioctl(dev_buf.devFD, DISABLE_CMD, devEvents[i]) < 0)
      perror("ioctl(DISABLE_CMD)");
    else{
      disabled++;
    }
  }
  return  disabled == this->nbrEvents;
}

void LitmusDevice::startTracingAllDevices() {

  // notice that starting to trace on one device
  // is enough since it will eventually call scheduleTrace()
  devQueue.front()->startTracing();
}

void  LitmusDevice::stopAllDevices() {
  do {
    devQueue.front()->stopTracing();
    delete devQueue.front();
    devQueue.pop();
  } while(devQueue.size()!=0);
}
