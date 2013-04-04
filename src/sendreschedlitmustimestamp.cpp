#include "sendreschedlitmustimestamp.hpp"

SendReschedLitmusTimestamp::SendReschedLitmusTimestamp(cmd_t id)
  : LitmusTimestamp(id) {

  state =  WAIT_FOR_START_EVENT;
}


void SendReschedLitmusTimestamp::check(struct timestamp* ts) {
  // Notice that for SEND_RESCHED, we don't care who sent the IPI
  // since real-time task will be affected by it anyway
  if ( (state == WAIT_FOR_START_EVENT)
       && (ts->event == startID)) {

    setLastSeqNo(ts->seq_no);
    state = WAIT_FOR_MATCH;
    currentTimestamp = *ts;
  }

  // Here currentTimestamp contains a start event and ts 
  // is begin event; Make sure ts was generated 
  // after currentTimestamp and set currentTimestamp to ts
  else if ((currentTimestamp.event == startID)
	   &&( ts->event == startID)
	   &&(currentTimestamp.seq_no < ts->seq_no)) {

    setLastSeqNo(ts->seq_no);	  
    state = WAIT_FOR_MATCH;
    currentTimestamp = *ts;
  }

  // Here currentTimestamp contains a start event and 
  // ts is an end event. Make sure ts was generated afer 
  // currentTimestamp. Then generate a new overhead value

  // Notice that we are sure this is the matching end event
  // since another starting event would take the state machine 
  // to the initial state(WAIT_FOR_MATCH), whereas an end event 
  // with a diffrent cpu would not go be checked by this state 
  // machine (see TimestampProcessor::processRegisteredTimestamp)

  else if ((currentTimestamp.event == startID)
	   &&(ts->event == startID+1)
	   &&(currentTimestamp.seq_no < ts->seq_no)
	   &&(getLastSeqNo()+1 == ts->seq_no)) {
	  
    setLastSeqNo(ts->seq_no);
    state = WAIT_FOR_START_EVENT;
    updateLitmusTimestampObservers((ts->timestamp - currentTimestamp.timestamp),
				   currentTimestamp.event );
  }
}
