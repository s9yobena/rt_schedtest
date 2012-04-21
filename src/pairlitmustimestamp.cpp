#include "pairlitmustimestamp.hpp"

PairLitmusTimestamp::PairLitmusTimestamp(cmd_t id) 
  : LitmusTimestamp(id) {

  this->currentTimestamp.resize(MAX_NBR_CPU,CPUTimestamp());
}

void PairLitmusTimestamp::check(struct timestamp* ts) {
  // Check if we are in the default state and store ts if it is a start event
  if ( (currentTimestamp[ts->cpu].state==0) && 
       (ts->event == this->startID)) {

    currentTimestamp[ts->cpu].state=1;
    currentTimestamp[ts->cpu].ts = *ts;
  }

  // Here currentTimestamp already contains a start event,
  // check if ts is a begin event; if so make sure it was generated 
  // after currentTimestamp and set currentTimestamp to ts
  else if ((currentTimestamp[ts->cpu].ts.event == startID)
	     &&( ts->event == startID)
	     &&(currentTimestamp[ts->cpu].ts.seq_no < ts->seq_no)) {

    currentTimestamp[ts->cpu].state=1;
    currentTimestamp[ts->cpu].ts = *ts;
  }

  // Here currentTimestamp contains a start event and 
  // ts is an end event. Make sure ts was generated afer 
  // currentTimestamp and its task_type ==TSK_RT. Then 
  // generate a new overhead value
  else if ((currentTimestamp[ts->cpu].ts.event == startID)
	   &&(ts->event == startID+1)
	   &&(ts->task_type == TSK_RT)
	   &&(currentTimestamp[ts->cpu].ts.seq_no < ts->seq_no)) {

    currentTimestamp[ts->cpu].state=0;
    std::cout<<"current overhead for "<<this->startID<<" is: \t"<<(ts->timestamp - currentTimestamp[ts->cpu].ts.timestamp)<<std::endl;
    updateLitmusTimestampObservers((ts->timestamp - currentTimestamp[ts->cpu].ts.timestamp),
				   currentTimestamp[ts->cpu].ts.event );
  }
}
