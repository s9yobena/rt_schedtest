#include "timestampprocessor.hpp"
#include "safetymargin.hpp"

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

  if (!isRegisteredTimestamp(ts))
    registerTimestamp(ts);

  processRegisteredTimestamp(ts);

  if (printTimestamps) {
  cout<<"timestamp: "<<(uint64_t)ts->timestamp<<"\t"
      <<"seq_no: "<<(int)ts->seq_no<<"\t"
      <<"cpu: "<<(int)ts->cpu<<"\t"
      <<"event: "<<(int)ts->event<<"\t"
      <<"task_type"<<(int)ts->task_type<<"\t"
      <<endl;     	  
  }

}

void TimestampProcessor::setTimestampProcessorObserver(LitmusOverhead* litmusOverhead) {
  this->litmusOverhead = litmusOverhead;
}

bool TimestampProcessor::isRegisteredTimestamp(struct timestamp* ts) {

  if (registeredLitmusTimestamps.find(pair<cmd_t,uint8_t>(ts->event,ts->cpu))!=registeredLitmusTimestamps.end())
    return true;
  else
    return false;

}
void TimestampProcessor::registerTimestamp(struct timestamp* ts) {

  map<pair<cmd_t,uint8_t>,LitmusTimestamp*>::iterator it;

  if (ts->event < SINGLE_RECORDS_RANGE) {
    
    registerPairLitmusTimestamp(ts);
  } else {
    
    registerSingleLitmusTimestamp(ts);
  }
}

void TimestampProcessor::registerPairLitmusTimestamp(struct timestamp* ts) {
  LitmusTimestamp *litmusTimestamp;
  map<pair<cmd_t,uint8_t>,LitmusTimestamp*>::iterator it;
  bool proceedRegister = false;

  // only look if this is a start event, by convention, 
  // the id of the start event is always even
  if ((ts->event % 2 == 0)
      
      &&((ts->event != TS_SEND_RESCHED_START)
	 &&(ts->event != TS_SEND_RESCHED_END))

      &&((ts->event != TS_TICK_START)
	 &&(ts->event != TS_TICK_END))) {

    litmusTimestamp = new PairLitmusTimestamp(ts->event);
    proceedRegister = true;
  } else if ( ts->event == TS_SEND_RESCHED_START) {

    litmusTimestamp = new SendReschedLitmusTimestamp(ts->event);
    proceedRegister = true;
  } else if ( ts->event == TS_TICK_START) {
    litmusTimestamp = new TickLitmusTimestamp(ts->event);
    proceedRegister = true;
  } 

  if (proceedRegister) {


    // The start and end timestamps share the same state:

    // register the start event
    it = registeredLitmusTimestamps.begin();
    registeredLitmusTimestamps.insert(it, pair<pair<cmd_t,uint8_t>,LitmusTimestamp*>
				      (pair<cmd_t,uint8_t>(ts->event,ts->cpu),litmusTimestamp));

    // register the end event, which is start event +1; notice that litmusTimestamp is the same
    it = registeredLitmusTimestamps.begin();
    registeredLitmusTimestamps.insert(it, pair<pair<cmd_t,uint8_t>,LitmusTimestamp*>
				      (pair<cmd_t,uint8_t>(ts->event+1,ts->cpu),litmusTimestamp));

    litmusTimestamp->setLitmusTimestampObserver(this);
  }
}

void TimestampProcessor::registerSingleLitmusTimestamp(struct timestamp* ts) {
  
  LitmusTimestamp *litmusTimestamp;
  map<pair<cmd_t,uint8_t>,LitmusTimestamp*>::iterator it;
    
  it = registeredLitmusTimestamps.begin();
  litmusTimestamp = new SingleLitmusTimestamp(ts->event);
  registeredLitmusTimestamps.insert(it, pair<pair<cmd_t,uint8_t>,LitmusTimestamp*>
				    (pair<cmd_t,uint8_t>(ts->event,ts->cpu),litmusTimestamp));

  litmusTimestamp->setLitmusTimestampObserver(this);
}

void TimestampProcessor::processRegisteredTimestamp(struct timestamp* ts) {
  
  registeredLitmusTimestamps[pair<cmd_t,uint8_t>(ts->event, ts->cpu)]->check(ts);
}



void TimestampProcessor::notifyNewOverhead(overhead_t overhead, cmd_t id) {
  
  if (printOverheads) {
     
    std::cout<<"current overhead for "<<id<<" is: \t"<<overhead<<std::endl;
  }

  overhead = SafetyMargin::makeSM(overhead);
  
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
  case TS_LOCK_SUSPEND:
    litmusOverhead->updateSumSelfSuspension(overhead);
    break;
  case TS_TICK_START:
    litmusOverhead->checkMaxTICK(overhead);
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
