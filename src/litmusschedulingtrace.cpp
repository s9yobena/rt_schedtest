#include "litmusschedulingtrace.hpp"

LitmusSchedulingTrace::LitmusSchedulingTrace()
  :LitmusDevice() {

}

void LitmusSchedulingTrace::setDefaultConfig() {
   enableEvent("503");
   enableEvent("506");		// 506 enables event event 7 (ST_COMPLETION ?!!
   schedulingTraceProcessor.setPrintSchedulingTraces(printSchedulingTraces);
   schedulingTraceProcessor.setPrintExecutionTimes(printExecutionTimes);
}

void LitmusSchedulingTrace::trace() {

  int rd;
  size_t size, count;
  struct st_event_record *st_er, *end;

  if ((rd = read(devFD, devBuffer, DEV_BUF_SIZE)) > 0) {

    size = rd;
    st_er    = (struct st_event_record*) devBuffer;
    count = size / sizeof(struct st_event_record);
    end   = st_er + count;
    for (; st_er != end; st_er++){
      schedulingTraceProcessor.processSchedulingTrace(st_er);
    }
  }

  // the base class LitmusDevie has a queue of devices, from which another device is scheduled to trace
  scheduleTrace();
}

int LitmusSchedulingTrace::eventStrToEventId(const char* eventStr, EventId *eventId) {
  
  *eventId = atoi(eventStr);
  return 0;
}

void LitmusSchedulingTrace::setParameters(const CmdlParser& cmdlParser) {

  this->printSchedulingTraces = cmdlParser.printSchedulingTraces;
  this->printExecutionTimes = cmdlParser.printExecutionTimes;
}


