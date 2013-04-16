#include "litmusschedulingtrace.hpp"

LitmusSchedulingTrace::LitmusSchedulingTrace(TaskSet *_taskSet)
  :LitmusDevice() {

  taskSet = _taskSet;
}

void LitmusSchedulingTrace::setDefaultConfig() {

  printSchedulingTraces = false;
  printExecutionTimes = false;

  enableEvent("503");           // 503 enables event event 3 (ST_RELEASE)
  enableEvent("506");		// 506 enables event event 7 (ST_COMPLETION ?!!
  enableEvent("507");		// 507 enables event event 8 (ST_BLOCK ?!!
  enableEvent("508");		// 508 enables event event 9 (ST_RESUME ?!!
  enableEvent("510");		// 508 enables event event 11 (ST_TERMINATION ?!!
  enableEvent("511");		// 508 enables event event 11 (ST_TERMINATION ?!!


  schedulingTraceProcessor.setPrintSchedulingTraces(printSchedulingTraces);
  schedulingTraceProcessor.setPrintExecutionTimes(printExecutionTimes);
  schedulingTraceProcessor.setTaskSet(taskSet);
}

void LitmusSchedulingTrace::trace() {

  size_t size, count;
  struct st_event_record *st_er, *end;

  pthread_mutex_lock(&dev_buf.mutex);
	
  if (dev_buf.status == empty) {
    pthread_mutex_unlock(&dev_buf.mutex);

    // no data read by our asynch_reader; in this case, we proceed to reading 
    // from the next device, wihout blocking.
    sleep(1);
    scheduleTrace();
  } else if (dev_buf.status == full) {
    
    size = dev_buf.size;
    st_er    = (struct st_event_record*) dev_buf.devBuffer;
    count = size / sizeof(struct st_event_record);
    end   = st_er + count;
    for (; st_er != end; st_er++){
      schedulingTraceProcessor.processSchedulingTrace(st_er);
    }

    dev_buf.status = empty;
    pthread_cond_signal(&dev_buf.empty);

    pthread_mutex_unlock(&dev_buf.mutex);
    scheduleTrace();
  }
}

int LitmusSchedulingTrace::eventStrToEventId(const char* eventStr, EventId *eventId) {
  
  *eventId = atoi(eventStr);
  return 0;
}

void LitmusSchedulingTrace::setParameters(const CmdlParser& cmdlParser) {

  this->printSchedulingTraces = cmdlParser.printSchedulingTraces;
  this->printExecutionTimes = cmdlParser.printExecutionTimes;
}


