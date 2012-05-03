#include "sendreschedlitmustimestamp.hpp"

SendReschedLitmusTimestamp::SendReschedLitmusTimestamp(cmd_t id)
  : LitmusTimestamp(id) {

  state =  WAIT_FOR_START_EVENT;
}


void SendReschedLitmusTimestamp::check(struct timestamp* ts) {
  // Notice that for SEND_RESCHED, we make sure that the start event is 
  // send from a real-time task
  if ( (state == WAIT_FOR_START_EVENT)
       && (ts->event == startID)
       && (ts->task_type == TSK_RT)) {

    state = WAIT_FOR_MATCH;
    currentTimestamp = *ts;
  }

  // Here currentTimestamp contains a start event and ts 
  // is begin event; Make sure ts was generated 
  // after currentTimestamp and set currentTimestamp to ts
  else if ((currentTimestamp.event == startID)
	   &&( ts->event == startID)
	   &&(currentTimestamp.seq_no < ts->seq_no)) {

    state = WAIT_FOR_MATCH;
    currentTimestamp = *ts;
  }

  // Here currentTimestamp contains a start event and 
  // ts is an end event. Make sure ts was generated afer 
  // currentTimestamp. Then generate a new overhead value

  // Notice that we don't check if the task is a real time one.
  else if ((currentTimestamp.event == startID)
	   &&(ts->event == startID+1)
	   &&(currentTimestamp.seq_no < ts->seq_no)) {

    state = WAIT_FOR_START_EVENT;
    updateLitmusTimestampObservers((ts->timestamp - currentTimestamp.timestamp),
				   currentTimestamp.event );
  }
}
