#include "litmusinterarrivaltime.hpp"

LitmusInterArrivalTime::LitmusInterArrivalTime(ster_t sterType) 
  : LitmusSchedulingTraceRecord(sterType) {
  
  state = IAT_WAIT_FOR_RELEASE_EVENT;
}


void LitmusInterArrivalTime::check(struct st_event_record* ster) {
  // Check if we are in WAIT_FOR_RELEASE_EVENT state; store st_event_record 
  if ( (state == IAT_WAIT_FOR_RELEASE_EVENT)
       && (ster->hdr.type == this->startID)) {

    state = IAT_WAIT_FOR_2ND_RELEASE_EVENT;
    currentStEventRecord = *ster;
  }

  // Here currentStEventRecord contains a release event and 
  // ster is a release event. Make sure ster's job number is exactly  
  // currentStEventRecord's number + 1. Then generate a new inter 
  // arrival time value.
  else if ((state == IAT_WAIT_FOR_2ND_RELEASE_EVENT)
	   &&(currentStEventRecord.hdr.type == startID)	   
	   && (ster->hdr.type == ST_RELEASE)
	   && (ster->hdr.job == currentStEventRecord.hdr.job + 1)) {

    state = IAT_WAIT_FOR_RELEASE_EVENT;
    updateTaskSet((uint64_t)(ster->data.release.release) - (uint64_t)(currentStEventRecord.data.release.release), currentStEventRecord.hdr.pid );
  }
  
  // Here currentStEventRecord contains a release event and ster 
  // is a release event whose job is not currentStEventRecord's number + 1 ;

  // WE DO SKIP THIS EVENT because inter-arrival time is the time between the release event
  // of one job and the release event of, exactly, the next job.
  else if ((state == IAT_WAIT_FOR_2ND_RELEASE_EVENT)
	   &&(currentStEventRecord.hdr.type == startID)
	   && (ster->hdr.type == startID)
    	   && (ster->hdr.job > currentStEventRecord.hdr.job )) {

    state = IAT_WAIT_FOR_RELEASE_EVENT;
  }
}

void LitmusInterArrivalTime::updateTaskSet(lt_t inter_arrival_time, pid_t task_id) {
  taskSet->updateTaskInterArrivalTime(inter_arrival_time, task_id);
}


