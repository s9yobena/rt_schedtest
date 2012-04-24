#include "timestampprocessor.hpp"

TimestampProcessor* TimestampProcessor::timestampProcessorInstance = 0;

TimestampProcessor::TimestampProcessor(){

}

TimestampProcessor* TimestampProcessor::getInstance(){
  if(!timestampProcessorInstance){
    timestampProcessorInstance = new TimestampProcessor();
  }
  return timestampProcessorInstance;
}

void TimestampProcessor::processTimestamp(struct timestamp* ts) {
  if (printTimestamps) {
  cout<<"timestamp: "<<(uint64_t)ts->timestamp<<"\t"
      <<"seq_no: "<<(int)ts->seq_no<<"\t"
      <<"cpu: "<<(int)ts->cpu<<"\t"
      <<"event: "<<(int)ts->event<<"\t"
      <<"task_type"<<(int)ts->task_type<<"\t"
      <<endl;     	  
  }

    registeredLitmusTimestamps[ts->event]->check(ts); // polymorphism is cool!
}

void TimestampProcessor::setTimestampProcessorObserver(LitmusOverhead* litmusOverhead) {
  this->litmusOverhead = litmusOverhead;
}

void TimestampProcessor::registerEvent(char* eventID) {
  
  cmd_t id;
  LitmusTimestamp *litmusTimestamp;
  SingleLitmusTimestamp* singleLitmusTimestamp;
  PairLitmusTimestamp* pairLitmusTimestamp;
  if (!str2event(eventID, &id)) {
    errno = EINVAL;
    // return 0; TODO: handle this case
  }
  registeredEvents.insert(id);
  if (id < SINGLE_RECORDS_RANGE) {
    litmusTimestamp = new PairLitmusTimestamp(id);
    // The start and end timestamps share the same state
    registeredLitmusTimestamps.insert(pair<cmd_t,LitmusTimestamp*>(id,litmusTimestamp)); // polymorphism is cool!
    registeredLitmusTimestamps.insert(pair<cmd_t,LitmusTimestamp*>((id+1),litmusTimestamp)); // polymorphism is cool!
  } else {
    litmusTimestamp = new SingleLitmusTimestamp(id);
    registeredLitmusTimestamps.insert(pair<cmd_t,LitmusTimestamp*>(id,litmusTimestamp)); // polymorphism is cool!
  }
  
  // Wait for new overheads from this timestamp
  litmusTimestamp->setLitmusTimestampObserver(this);
  
  std::cout<<"TimestampProcessor::registerEvent: added event "<<id<<std::endl;
} 

void TimestampProcessor::notifyNewOverhead(overhead_t overhead, cmd_t id) {
  
  if (printOverheads) {
     
    std::cout<<"current overhead for "<<id<<" is: \t"<<overhead<<std::endl;
  }
  
  switch(id){
  case TS_CXS_START:
    litmusOverhead->checkMaxCXS(overhead);
    break;
  case TS_SCHED_START:
    litmusOverhead->checkMaxSCHED(overhead);
    break;
  case TS_SCHED2_START:
    litmusOverhead->checkMaxSCHED2(overhead);
    break;
  case TS_RELEASE_START:
    litmusOverhead->checkMaxRELEASE(overhead);
    break;
  case TS_SEND_RESCHED_START:
    litmusOverhead->checkMaxSEND_RESCHED(overhead);
    break;
  case TS_RELEASE_LATENCY:
    litmusOverhead->checkMaxRELEASE_LATENCY(overhead);
    break;
    
  default:
    cout<<"WARNING: case not handeled"<<endl;
  }

}

void TimestampProcessor::setPrintTimestamps(bool printTimestamps) {

  this->printTimestamps = printTimestamps;
}

void TimestampProcessor::setPrintOverheads(bool printOverheads) {
  this->printOverheads = printOverheads;
}
