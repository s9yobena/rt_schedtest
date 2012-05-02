#include "litmusinterarrivaltime.hpp"

LitmusInterArrivalTime::LitmusInterArrivalTime(ster_t sterType) 
  : LitmusSchedulingTraceRecord(sterType) {
  
  state = WAIT_FOR_COMPLETION_EVENT;
  taskSet = TaskSet::getInstance();
}


void LitmusInterArrivalTime::check(struct st_event_record* ster) {
  // Check if we are in WAIT_FOR_COMPLETION_EVENT state; store st_event_record 
  if ( (state == WAIT_FOR_COMPLETION_EVENT)
       && (ster->hdr.type == this->startID)) {

    state = WAIT_FOR_RELEASE_EVENT;
    currentStEventRecord = *ster;
  }

  // Here currentStEventRecord contains a completion event and ster 
  // is a completion event; Make sure ster's job has completed after
  // currentStEventRecord'S one; Overwite currentStEventRecord.
  
  // TODO: Should we save the old currentStEventRecord and wait for its release event ?
  else if ((currentStEventRecord.hdr.type == startID)
	   && (ster->hdr.type == startID)
    	   && (ster->hdr.job > currentStEventRecord.hdr.job )) {

    state = WAIT_FOR_RELEASE_EVENT;
    currentStEventRecord = *ster;
  }

  // Here currentStEventRecord contains a completion event and 
  // ster is a release event. Make sure ster's job number is exactly  
  // currentStEventRecord's number + 1. Then generate a new inter 
  // arrival time value.
  else if ((currentStEventRecord.hdr.type == startID)	   
	   && (ster->hdr.type == ST_RELEASE)
	   && (ster->hdr.job == currentStEventRecord.hdr.job + 1)) {

    state = WAIT_FOR_COMPLETION_EVENT;
    updateLitmusSchedulingTraceRecordObservers((uint64_t)(ster->data.completion.when) - (uint64_t)(currentStEventRecord.data.release.release), currentStEventRecord.hdr.pid );
  }
}

void LitmusInterArrivalTime::updateLitmusSchedulingTraceRecordObservers(exec_time_t inter_arrival_time, task_id_t task_id) {
  taskSet->updateTaskInterArrivalTime(inter_arrival_time,task_id);
}




