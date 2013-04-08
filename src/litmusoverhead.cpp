#include "litmusoverhead.hpp"

LitmusOverhead* LitmusOverhead::litmusOverheadInstance = 0;


LitmusOverhead::LitmusOverhead()
  :LitmusDevice() {
}

LitmusOverhead* LitmusOverhead::getInstance(){
  if(!litmusOverheadInstance){
    litmusOverheadInstance = new LitmusOverhead();
  }
  return litmusOverheadInstance;
}

void LitmusOverhead::setDefaultConfig() {

  printTimestamps = false;
  printOverheads = false;

  maxCXS = 0;
  maxSCHED = 0;
  maxSCHED2 = 0;
  maxRELEASE = 0;
  maxRELEASE_LATENCY = 0;
  maxSEND_RESCHED = 0;
  maxTICK = 0;
  sumSelfSuspension = 0;
  
  timestampProcessor = TimestampProcessor::getInstance();
  timestampProcessor->setPrintTimestamps(printTimestamps);
  timestampProcessor->setPrintOverheads(printOverheads);
  timestampProcessor->setTimestampProcessorObserver(this);

  schedTestParam = SchedTestParam::getInstance();
  litmusSchedTest = LitmusSchedTest::getInstance();

  // When the tracign daemon starts executing, the maximum system overheads
  // may not be 0 (if system was already running) in this case the daemon may 
  // work with 0 overheads until litmus exports an overheads value that is a 
  // maximum value with regard to litmus internal maximum overheads. 
  // To avoid this gap, this call makes sure we start we 0 overheads.
  reset_max_overheads();


  enableEvent("CXS_START");
  enableEvent("CXS_END");

  enableEvent("SCHED_START");
  enableEvent("SCHED_END");

  enableEvent("SCHED2_START");
  enableEvent("SCHED2_END");

  enableEvent("SEND_RESCHED_START");
  enableEvent("SEND_RESCHED_END");

  enableEvent("RELEASE_START");
  enableEvent("RELEASE_END");

  enableEvent("LOCK_SUSPEND");
  enableEvent("LOCK_RESUME");

  enableEvent("TICK_START");
  enableEvent("TICK_END");
  
  enableEvent("RELEASE_LATENCY");
}

void LitmusOverhead::trace() {

  int rd;
  size_t size, count;
  struct timestamp *ts, *end;

  if ((rd = read(devFD, devBuffer, DEV_BUF_SIZE)) > 0) {

    size = rd;
    ts    = (struct timestamp*) devBuffer;
    count = size / sizeof(struct timestamp);
    end   = ts + count;
		
    for (; ts != end; ts++){
      timestampProcessor->processTimestamp(ts);
    }
  }
  
  // the base class LitmusDevie has a queue of devices, from which another device is scheduled to trace
  scheduleTrace();

}

int LitmusOverhead::eventStrToEventId(const char* eventStr, EventId *eventId) {
  
  if (!str2event(eventStr, eventId)) {
    errno = EINVAL;
    return 1;
  }
  return 0;
}

void LitmusOverhead::setParameters(const CmdlParser& cmdlParser) {
  this->printTimestamps = cmdlParser.printTimestamps;
  this->printOverheads = cmdlParser.printOverheads;
}

void LitmusOverhead::checkMaxCXS(overhead_t cxsOverhead) {
  if (this->maxCXS < cxsOverhead){
    this->maxCXS = cxsOverhead;
    schedTestParam->setCXS(this->maxCXS);
    schedTestParam->makeSchedTestParam();
    litmusSchedTest->callSchedTest(schedTestParam->getOutputName());
  }
}

void LitmusOverhead::checkMaxSCHED(overhead_t schedOverhead) {
  if (this->maxSCHED < schedOverhead){
    this->maxSCHED = schedOverhead;
    schedTestParam->setSCHED(this->maxSCHED);
    schedTestParam->makeSchedTestParam();
    litmusSchedTest->callSchedTest(schedTestParam->getOutputName());
  }
}

void LitmusOverhead::checkMaxSCHED2(overhead_t sched2Overhead) {
  if (this->maxSCHED2 < sched2Overhead){
    this->maxSCHED2 = sched2Overhead;
    schedTestParam->setSCHED2(this->maxSCHED2);
    schedTestParam->makeSchedTestParam();
    litmusSchedTest->callSchedTest(schedTestParam->getOutputName());
  }
}

void LitmusOverhead::checkMaxRELEASE(overhead_t releaseOverhead) {
  if (this->maxRELEASE < releaseOverhead){
    this->maxRELEASE = releaseOverhead;
    schedTestParam->setRELEASE(this->maxRELEASE);
    schedTestParam->makeSchedTestParam();
    litmusSchedTest->callSchedTest(schedTestParam->getOutputName());
  }
}

void LitmusOverhead::checkMaxSEND_RESCHED(overhead_t send_reschedOverhead) {
  if (this->maxSEND_RESCHED < send_reschedOverhead){
    this->maxSEND_RESCHED = send_reschedOverhead;
    schedTestParam->setSEND_RESCHED(this->maxSEND_RESCHED);
    schedTestParam->makeSchedTestParam();
    litmusSchedTest->callSchedTest(schedTestParam->getOutputName());
  }
}

void LitmusOverhead::checkMaxRELEASE_LATENCY(overhead_t release_latencyOverhead) {
  if (this->maxRELEASE_LATENCY < release_latencyOverhead){
    this->maxRELEASE_LATENCY = release_latencyOverhead;
    schedTestParam->setRELEASE_LATENCY(this->maxRELEASE_LATENCY);
    schedTestParam->makeSchedTestParam();
    litmusSchedTest->callSchedTest(schedTestParam->getOutputName());
  }
}

void LitmusOverhead::updateSumSelfSuspension(overhead_t selfSuspension) {
  sumSelfSuspension += selfSuspension;
}

void LitmusOverhead::checkMaxTICK(overhead_t tickOverhead) {
  if (this->maxTICK < tickOverhead){
    this->maxTICK = tickOverhead;
    schedTestParam->setTICK(this->maxTICK);
    schedTestParam->makeSchedTestParam();
    litmusSchedTest->callSchedTest(schedTestParam->getOutputName());
  }
}

