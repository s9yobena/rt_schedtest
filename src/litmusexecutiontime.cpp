#include "litmusexecutiontime.hpp"
#include "safetymargin.hpp"

LitmusExecutionTime::LitmusExecutionTime(ster_t sterType) 
  : LitmusSchedulingTraceRecord(sterType) {

  state =  WAIT_FOR_RELEASE_EVENT;
  lastJobNo = 0;
  printDebug = false;
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
	   && (ster->hdr.job == currentStEventRecord.hdr.job )
	   // This check is needed to avoid adding a task that has terminated
	   && (ster->hdr.job > getLastJobNo())) {
    
    if (printDebug) {

      cout<<"Task execution of pid: "<<ster->hdr.pid
	  <<", JobNo: "<<ster->hdr.job
	  <<", cpu: "<<ster->hdr.cpu
	  <<", lastJobNo"<<getLastJobNo()
	  <<endl;    
    }
    
    state = WAIT_FOR_RELEASE_EVENT;
    updateTaskSet((uint64_t)(ster->data.completion.when) - (uint64_t)(currentStEventRecord.data.release.release)
		  ,currentStEventRecord.hdr.cpu
		  ,currentStEventRecord.hdr.pid );
  }


  else if (ster->hdr.type == ST_TERMINATION) {

    setLastJobNo(ster->hdr.job);

    if (printDebug) {
      
      cout<<"Task termination of pid: "<<ster->hdr.pid
	  <<", JobNo: "<<ster->hdr.job
	  <<", cpu: "<<ster->hdr.cpu
	  <<", lastJobNo"<<getLastJobNo()
	  <<endl;    
    }

    state = TASK_TERMINATED;
    taskSet->removeTask(ster->hdr.pid);
    // schedTestParam->makeSchedTestParam();
    // litmusSchedTest->callSchedTest(schedTestParam->getOutputName());
    litmusSchedTest->doSchedTest();

  }

  else if (state == TASK_TERMINATED
	   && (ster->hdr.job > getLastJobNo())) {

    // A new task is release whose pid matched a previously terminated task.
    state = WAIT_FOR_RELEASE_EVENT;
    
    if (printDebug) {
      
    cout<<"New task release with same pid pid: "<<ster->hdr.pid<<", JobNo: "<<ster->hdr.job
	<<", lastJobNo"<<getLastJobNo()
	<<endl;    
    }

  }

}


void LitmusExecutionTime::updateTaskSet(lt_t exec_time, unsigned _cpu, pid_t task_id) {

  exec_time = SafetyMargin::makeSM(exec_time);

  // Uncomment the following instruction to enable computing a task's average
  // execution cost. Be careful, however, as this increases the response time 
  // of the tracing-daemon.
  // lt_t avrgExecCost = taskSet->computeAverageExecCost();
  // taskSet->setAverageExecCost(avrgExecCost);

  // Whenever a task migrates to another cpu or  exhibits a 
  // larger execution time, a new schedulability test is performed.
  
  // Warning: in case of clustered or partitioned scheduling, uncomment the cpu 
  // check, as task migrations may affect schedulability.
  if (taskSet->updateTaskExecCost(exec_time, task_id)) {
  // || _cpu != taskSet->getTaskCpu(task_id) ){
    taskSet->setTaskCpu(task_id, _cpu);
    // schedTestParam->makeSchedTestParam();
    // litmusSchedTest->callSchedTest(schedTestParam->getOutputName());
  
    litmusSchedTest->doSchedTest();
  }
}

void LitmusExecutionTime::setLastJobNo(unsigned long long _lastJobNo) {

  lastJobNo = _lastJobNo;
}

unsigned long long LitmusExecutionTime::getLastJobNo() {

  return lastJobNo;
}
