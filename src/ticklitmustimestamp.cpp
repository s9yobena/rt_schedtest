#include "ticklitmustimestamp.hpp"

TickLitmusTimestamp::TickLitmusTimestamp(cmd_t id) 
  : LitmusTimestamp(id) {

  state =  WAIT_FOR_START_EVENT;
}


// The "state machine" can be in two states:

// in the beggining, it is in the WAIT_FOR_START_EVENT in which 
// the first start event takes the  "machine" to the WAIT_FOR_MATCH state.

// In WAIT_FOR_MATCH, the machine looks for a matching timesatmp for the one 
// set in the WAIT_FOR_START_EVENT state; There are two possibilites: 1) either
// a timestamp with an end event is found, in which case a new overhead is 
// generated. 2) another start event is found, in which case it overwrites the 
// previous begin event(see below for the condition).

void TickLitmusTimestamp::check(struct timestamp* ts) {
  // Check if we are in WAIT_FOR_START_EVENT state; store the timestamp ts
  if ( (state == WAIT_FOR_START_EVENT) && 
       (ts->event == this->startID)) {

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
  // currentTimestamp.
  // Note that we don't care about the task type.
  // Then, we generate a new overhead value
  else if ((currentTimestamp.event == startID)
	   &&(ts->event == startID+1)
	   &&(currentTimestamp.seq_no < ts->seq_no)) {

    state = WAIT_FOR_START_EVENT;
    updateLitmusTimestampObservers((ts->timestamp - currentTimestamp.timestamp),
				   currentTimestamp.event );
  }
}
