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
  
  timestampProcessor = TimestampProcessor::getInstance();
  timestampProcessor->setPrintTimestamps(printTimestamps);
  timestampProcessor->setPrintOverheads(printOverheads);
  timestampProcessor->setTimestampProcessorObserver(this);

  packet2Stdout = Packet2Stdout::getInstance();

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

  enableEvent("RELEASE_LATENCY");
}

void LitmusOverhead::trace() {

  int rd;
  size_t size, count;
  struct timestamp *ts, *end;
  overhead_t currentOverhead;
  unsigned long currentOverheadId;

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
    packet2Stdout->setCXS(this->maxCXS);
    packet2Stdout->send();
  }
}

void LitmusOverhead::checkMaxSCHED(overhead_t schedOverhead) {
  if (this->maxSCHED < schedOverhead){
    this->maxSCHED = schedOverhead;
    packet2Stdout->setSCHED(this->maxSCHED);
    packet2Stdout->send();
  }
}

void LitmusOverhead::checkMaxSCHED2(overhead_t sched2Overhead) {
  if (this->maxSCHED2 < sched2Overhead){
    this->maxSCHED2 = sched2Overhead;
    packet2Stdout->setSCHED2(this->maxSCHED2);
    packet2Stdout->send();
  }
}

void LitmusOverhead::checkMaxRELEASE(overhead_t releaseOverhead) {
  if (this->maxRELEASE < releaseOverhead){
    this->maxRELEASE = releaseOverhead;
    packet2Stdout->setRELEASE(this->maxRELEASE);
    packet2Stdout->send();
  }
}

void LitmusOverhead::checkMaxSEND_RESCHED(overhead_t send_reschedOverhead) {
  if (this->maxSEND_RESCHED < send_reschedOverhead){
    this->maxSEND_RESCHED = send_reschedOverhead;
    packet2Stdout->setSEND_RESCHED(this->maxSEND_RESCHED);
    packet2Stdout->send();
  }
}

void LitmusOverhead::checkMaxRELEASE_LATENCY(overhead_t release_latencyOverhead) {
  if (this->maxRELEASE_LATENCY < release_latencyOverhead){
    this->maxRELEASE_LATENCY = release_latencyOverhead;
    packet2Stdout->setRELEASE_LATENCY(this->maxRELEASE_LATENCY);
    packet2Stdout->send();
  }
}
