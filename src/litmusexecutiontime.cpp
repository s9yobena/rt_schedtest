#include "litmusexecutiontime.hpp"

LitmusExecutionTime::LitmusExecutionTime(ster_t sterType) 
  : LitmusSchedulingTraceRecord(sterType) {

  state =  WAIT_FOR_RELEASE_EVENT;
}

void LitmusExecutionTime::check(struct st_event_record* ster) {
  // Check if we are in WAIT_FOR_RELEASE_EVENT state; store st_event_record 
  if ( (state == WAIT_FOR_RELEASE_EVENT)
       && (ster->hdr.type == this->startID)) {

    state = WAIT_FOR_COMPLETION_EVENT;
    currentStEventRecord = *ster;
  }

  // Here currentStEventRecord contains a release event and ster 
  // is a release event; Make sure ster's job was released after
  // currentStEventRecord'S one; Overwite currentStEventRecord.
  
  // TODO: Should we save the old currentStEventRecord and wait for its completion event ?
  else if ((currentStEventRecord.hdr.type == startID)
	   && (ster->hdr.type == startID)
    	   && (ster->hdr.job > currentStEventRecord.hdr.job )) {

    state = WAIT_FOR_COMPLETION_EVENT;
    currentStEventRecord = *ster;
  }

  // Here currentStEventRecord contains a release event and 
  // ster is a completion event. Make sure ster's job matches 
  // currentStEventRecord's one. Then generate a new execution time value
  else if ((currentStEventRecord.hdr.type == startID)	   
	   && (ster->hdr.type == ST_COMPLETION)
	   && (ster->hdr.job == currentStEventRecord.hdr.job )) {

    state = WAIT_FOR_RELEASE_EVENT;
    updateLitmusSchedulingTraceRecordObservers((uint64_t)(ster->data.completion.when) - (uint64_t)(currentStEventRecord.data.release.release), currentStEventRecord.hdr.pid );
  }
}
