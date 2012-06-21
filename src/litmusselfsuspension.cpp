#include "litmusselfsuspension.hpp"

LitmusSelfSuspension::LitmusSelfSuspension(ster_t sterType) 
  : LitmusSchedulingTraceRecord(sterType) {

  state =  WAIT_FOR_BLOCK_EVENT;
}

void LitmusSelfSuspension::check(struct st_event_record* ster) {
  // Check if we are in WAIT_FOR_BLOCK_EVENT state; store st_event_record 
  if ( (state == WAIT_FOR_BLOCK_EVENT)
       && (ster->hdr.type == this->startID)) {

    state = WAIT_FOR_RESUME_EVENT;
    currentStEventRecord = *ster;
  }

  // Here currentStEventRecord contains a BLOCK event and ster 
  // is a BLOCK event; Make sure ster's job was released after
  // currentStEventRecord'S one; Overwite currentStEventRecord.
  
  // TODO: Should we save the old currentStEventRecord and wait for its RESUME event ?
  else if ((currentStEventRecord.hdr.type == startID)
	   && (ster->hdr.type == startID)
    	   && (ster->hdr.job > currentStEventRecord.hdr.job )) {

    state = WAIT_FOR_RESUME_EVENT;
    currentStEventRecord = *ster;
  }

  // Here currentStEventRecord contains a BLOCK event and 
  // ster is a completion event. Make sure ster's job matches 
  // currentStEventRecord's one. Then generate a new execution time value
  else if ((currentStEventRecord.hdr.type == startID)	   
	   && (ster->hdr.type == ST_RESUME)
	   && (ster->hdr.job == currentStEventRecord.hdr.job )) {

    state = WAIT_FOR_BLOCK_EVENT;
    updateTaskSet((uint64_t)(ster->data.resume.when) - (uint64_t)(currentStEventRecord.data.block.when), currentStEventRecord.hdr.pid );
  }
}

void LitmusSelfSuspension::updateTaskSet(lt_t self_suspension_time, pid_t task_id) {
  taskSet->updateTaskSelfSuspension(self_suspension_time, task_id);
}
