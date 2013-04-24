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
    if ((registeredTraceRecords.find(pair<int,int>(ster->hdr.type,ster->hdr.pid))
	 !=registeredTraceRecords.end()
	 &&registeredInterArrivalTimeTraceRocords.find(pair<int,int>(ster->hdr.type,ster->hdr.pid))
	 !=registeredInterArrivalTimeTraceRocords.end())

	||registeredSelfSuspensions.find(pair<int,int>(ster->hdr.type,ster->hdr.pid))
	!=registeredSelfSuspensions.end()
	)
      return true;
    else if (ster->hdr.type == ST_TERMINATION)
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
  map<pair<int,int>,LitmusSchedulingTraceRecord*>::iterator it;

  litmusSchedulingTraceRecord = new LitmusExecutionTime(ST_RELEASE);
  litmusSchedulingTraceRecord->setTaskSet(taskSet);

  it = registeredTraceRecords.begin();
  registeredTraceRecords.insert(it, pair<pair<int,int>,LitmusSchedulingTraceRecord*>
				(pair<int,int>(ST_RELEASE,ster->hdr.pid),litmusSchedulingTraceRecord));

  // register the completion scheduling trace; notice that LitmusSchedulingTraceRecord is the same
  it = registeredTraceRecords.begin();
  registeredTraceRecords.insert(it,pair<pair<int,int>,LitmusSchedulingTraceRecord*>
				(pair<int,int>(ST_COMPLETION,ster->hdr.pid),litmusSchedulingTraceRecord));

  // register the termintation scheduling trace; notice that LitmusSchedulingTraceRecord is the same
  it = registeredTraceRecords.begin();
  registeredTraceRecords.insert(it,pair<pair<int,int>,LitmusSchedulingTraceRecord*>
				(pair<int,int>(ST_TERMINATION,ster->hdr.pid),litmusSchedulingTraceRecord));
}

void SchedulingTraceProcessor::registerLitmusInterArrivalTime(struct st_event_record* ster) {

  LitmusSchedulingTraceRecord *litmusSchedulingTraceRecord;
  map<pair<int,int>,LitmusSchedulingTraceRecord*>::iterator it;

  litmusSchedulingTraceRecord = new LitmusInterArrivalTime(ST_RELEASE);
  litmusSchedulingTraceRecord->setTaskSet(taskSet);

  it = registeredInterArrivalTimeTraceRocords.begin();
  registeredInterArrivalTimeTraceRocords.insert(it, pair<pair<int,int>,LitmusSchedulingTraceRecord*>
  				(pair<int,int>(ST_COMPLETION,ster->hdr.pid),litmusSchedulingTraceRecord));

  // register the release scheduling trace; notice that LitmusSchedulingTraceRecord is the same
  it = registeredInterArrivalTimeTraceRocords.begin();
  registeredInterArrivalTimeTraceRocords.insert(it,pair<pair<int,int>,LitmusSchedulingTraceRecord*>
  				(pair<int,int>(ST_RELEASE,ster->hdr.pid),litmusSchedulingTraceRecord));

}

void SchedulingTraceProcessor::registerLitmusSelfSuspension(struct st_event_record* ster) {

  LitmusSchedulingTraceRecord *litmusSchedulingTraceRecord;
  map<pair<int,int>,LitmusSchedulingTraceRecord*>::iterator it;

  litmusSchedulingTraceRecord = new LitmusSelfSuspension(ST_BLOCK);
  litmusSchedulingTraceRecord->setTaskSet(taskSet);

  it = registeredSelfSuspensions.begin();
  registeredSelfSuspensions.insert(it, pair<pair<int,int>,LitmusSchedulingTraceRecord*>
				(pair<int,int>(ST_BLOCK,ster->hdr.pid),litmusSchedulingTraceRecord));

  // register the completion scheduling trace; notice that LitmusSchedulingTraceRecord is the same
  it = registeredSelfSuspensions.begin();
  registeredSelfSuspensions.insert(it,pair<pair<int,int>,LitmusSchedulingTraceRecord*>
				(pair<int,int>(ST_RESUME,ster->hdr.pid),litmusSchedulingTraceRecord));
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
 
    this->registeredTraceRecords[pair<int,int>(ster->hdr.type, ster->hdr.pid)]->check(ster);
    this->registeredInterArrivalTimeTraceRocords[pair<int,int>(ster->hdr.type, ster->hdr.pid)]->check(ster);
  
  } else if (ster->hdr.type ==	ST_BLOCK || ster->hdr.type == ST_RESUME) {

    this->registeredSelfSuspensions[pair<int,int>(ster->hdr.type, ster->hdr.pid)]->check(ster);

  } else if (ster->hdr.type ==	ST_TERMINATION) {

    this->registeredTraceRecords[pair<int,int>(ster->hdr.type, ster->hdr.pid)]->check(ster);    
  }
}

void SchedulingTraceProcessor::setPrintSchedulingTraces(bool printSchedulingTraces) {

  this->printSchedulingTraces = printSchedulingTraces;
}

void SchedulingTraceProcessor::setPrintExecutionTimes(bool printExecutionTimes) {

  this->printExecutionTimes = printExecutionTimes;
}
