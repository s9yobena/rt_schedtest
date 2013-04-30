#include "schedulingtraceprocessor.hpp"

SchedulingTraceProcessor* 
SchedulingTraceProcessor::schedulingTraceProcessorInstance = 0;

SchedulingTraceProcessor::SchedulingTraceProcessor(){
}

SchedulingTraceProcessor* SchedulingTraceProcessor::getInstance() {

  if (!schedulingTraceProcessorInstance) {
    
    schedulingTraceProcessorInstance = new SchedulingTraceProcessor();
  }    
  return schedulingTraceProcessorInstance;
}

void SchedulingTraceProcessor::setTaskSet(TaskSet *_taskSet){
  
  taskSet = _taskSet;
}


void SchedulingTraceProcessor::processSchedulingTrace(struct st_event_record* ster) {

  if (printSchedulingTraces) {
    std::cout<<"Printting info for st_event_record: \n"<<endl;
    std::cout<<"type: "<<(int)ster->hdr.type<<"\t"
	     <<"cpu: "<<(int)ster->hdr.cpu<<"\t"
	     <<"pid: "<<(int)ster->hdr.pid<<"\t"
	     <<"job: "<<(int)ster->hdr.job<<"\t"
	     <<endl;

    switch(ster->hdr.type) {
    case ST_RELEASE:
      std::cout<<"job released at: "<<ster->data.release.release<<"\t"
	       <<endl;
      break;
    case ST_COMPLETION:
      std::cout<<"job completed at: "<<ster->data.completion.when<<"\t"
	       <<endl;
      break;
    }
  } 

  bool process;
  process = true;

  if (!isRegisteredSchedulingTrace(ster))
    registerSchedulingTrace(ster) ? process = true: process = false;

  if (process)
    processRegisteredSchedulingTrace(ster);

}

bool SchedulingTraceProcessor::isRegisteredSchedulingTrace(struct st_event_record* ster) {
  try {
    if (ster->hdr.type == ST_RELEASE || ster->hdr.type == ST_COMPLETION) {
      
      if ((registeredTraceRecords.find(ster->hdr.pid)
	   !=registeredTraceRecords.end()
	   &&registeredInterArrivalTimeTraceRocords.find(ster->hdr.pid)
	   !=registeredInterArrivalTimeTraceRocords.end()))
	return true;
      else 
	return false;
    
    } else if (ster->hdr.type == ST_BLOCK || ster->hdr.type == ST_RESUME) {
      
      if (registeredSelfSuspensions.find(ster->hdr.pid)
	  !=registeredSelfSuspensions.end())
	return true;
      else 
	return false;

    }  else if (ster->hdr.type == ST_TERMINATION)
      // we do not need to register termination traces, we just process them
      return true;
    else
      return false;
  } catch (const exception &e) {
    
    cout<<"Exception at SchedulingTraceProcessor::isRegisteredSchedulingTrace: "
	<<e.what()
	<<endl;
    return false;
  }
} 

void SchedulingTraceProcessor::registerLitmusExecutionTime(struct st_event_record* ster) {

  LitmusSchedulingTraceRecord *litmusSchedulingTraceRecord;

  litmusSchedulingTraceRecord = new LitmusExecutionTime(ST_RELEASE);
  litmusSchedulingTraceRecord->setTaskSet(taskSet);

  // Registers ST_RELEASE, ST_COMPLETION, ST_TERMINATION.
  registeredTraceRecords.insert(pair<int,LitmusSchedulingTraceRecord*>
				(ster->hdr.pid,litmusSchedulingTraceRecord));
}

void SchedulingTraceProcessor::registerLitmusInterArrivalTime(struct st_event_record* ster) {

  LitmusSchedulingTraceRecord *litmusSchedulingTraceRecord;

  litmusSchedulingTraceRecord = new LitmusInterArrivalTime(ST_RELEASE);
  litmusSchedulingTraceRecord->setTaskSet(taskSet);

  // Registers ST_RELEASE.
  registeredInterArrivalTimeTraceRocords.insert(pair<int,LitmusSchedulingTraceRecord*>
						  (ster->hdr.pid,litmusSchedulingTraceRecord));

}

void SchedulingTraceProcessor::registerLitmusSelfSuspension(struct st_event_record* ster) {

  LitmusSchedulingTraceRecord *litmusSchedulingTraceRecord;

  litmusSchedulingTraceRecord = new LitmusSelfSuspension(ST_BLOCK);
  litmusSchedulingTraceRecord->setTaskSet(taskSet);

  // Registers ST_BLOCK, ST_RESUME.
  registeredSelfSuspensions.insert(pair<int,LitmusSchedulingTraceRecord*>
				     (ster->hdr.pid,litmusSchedulingTraceRecord));
}

bool SchedulingTraceProcessor::registerSchedulingTrace(struct st_event_record* ster) {

  if (ster->hdr.type ==	ST_RELEASE || ster->hdr.type == ST_COMPLETION) {
    
    registerLitmusExecutionTime(ster);
    registerLitmusInterArrivalTime(ster);
    return true;
  } else if (ster->hdr.type ==	ST_BLOCK || ster->hdr.type == ST_RESUME) {
    
    registerLitmusSelfSuspension(ster);
    return true;
  } else {
    
    return false;
  }
  cout<<"We should not reach here: SchedulingTraceProcessor::registerSchedulingTrace"<<endl;
  exit(1);
}
void SchedulingTraceProcessor::processRegisteredSchedulingTrace(struct st_event_record* ster) {
  if (ster->hdr.type ==	ST_RELEASE || ster->hdr.type == ST_COMPLETION) {
 
    this->registeredTraceRecords[ster->hdr.pid]->check(ster);
    this->registeredInterArrivalTimeTraceRocords[ster->hdr.pid]->check(ster);
  
  } else if (ster->hdr.type ==	ST_BLOCK || ster->hdr.type == ST_RESUME) {

    this->registeredSelfSuspensions[ster->hdr.pid]->check(ster);

  } else if (ster->hdr.type ==	ST_TERMINATION) {

    this->registeredTraceRecords[ster->hdr.pid]->check(ster);    
  }
}

void SchedulingTraceProcessor::setPrintSchedulingTraces(bool printSchedulingTraces) {

  this->printSchedulingTraces = printSchedulingTraces;
}

void SchedulingTraceProcessor::setPrintExecutionTimes(bool printExecutionTimes) {

  this->printExecutionTimes = printExecutionTimes;
}
