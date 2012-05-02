#include "schedulingtraceprocessor.hpp"

SchedulingTraceProcessor::SchedulingTraceProcessor(){
  setSchedulingTraceProcessorObserver(TaskSet::getInstance());
}

void SchedulingTraceProcessor::processSchedulingTrace(struct st_event_record* ster) {

  if (printSchedulingTraces) {
  
    cout<<"Printting info for st_event_record: \n"<<endl;
    cout<<"type: "<<(int)ster->hdr.type<<"\t"
	<<"cpu: "<<(int)ster->hdr.cpu<<"\t"
	<<"pid: "<<(int)ster->hdr.pid<<"\t"
	<<"job: "<<(int)ster->hdr.job<<"\t"
	<<endl;

    switch(ster->hdr.type) {
    case ST_RELEASE:
      cout<<"job released at: "<<ster->data.release.release<<"\t"
	  <<endl;
      break;
    case ST_COMPLETION:
      cout<<"job completed at: "<<ster->data.completion.when<<"\t"
	  <<endl;
      break;
    }
  }

  if (!isRegisteredSchedulingTrace(ster))
    registerSchedulingTrace(ster);

  processRegisteredSchedulingTrace(ster);

}


void SchedulingTraceProcessor::setSchedulingTraceProcessorObserver(TaskSet* taskSet) {
  this->taskSet = taskSet;
}

bool SchedulingTraceProcessor::isRegisteredSchedulingTrace(struct st_event_record* ster) {

  if (registeredTraceRecords.find(pair<int,int>(ster->hdr.type,ster->hdr.pid))
      !=registeredTraceRecords.end())
    return true;
  else
    return false;
}

void SchedulingTraceProcessor::registerLitmusExecutionTime(struct st_event_record* ster) {

  LitmusSchedulingTraceRecord *litmusSchedulingTraceRecord;
  LitmusExecutionTime *litmusExecutionTime;
  map<pair<int,int>,LitmusSchedulingTraceRecord*>::iterator it;

  litmusSchedulingTraceRecord = new LitmusExecutionTime(ST_RELEASE);

  it = registeredTraceRecords.begin();
  registeredTraceRecords.insert(it, pair<pair<int,int>,LitmusSchedulingTraceRecord*>
				(pair<int,int>(ST_RELEASE,ster->hdr.pid),litmusSchedulingTraceRecord));

  // register the completion scheduling trace; notice that LitmusSchedulingTraceRecord is the same
  it = registeredTraceRecords.begin();
  registeredTraceRecords.insert(it,pair<pair<int,int>,LitmusSchedulingTraceRecord*>
				(pair<int,int>(ST_COMPLETION,ster->hdr.pid),litmusSchedulingTraceRecord));

  litmusSchedulingTraceRecord->setLitmusSchedulingTraceRecordObserver(this);
}

void SchedulingTraceProcessor::registerLitmusInterArrivalTime(struct st_event_record* ster) {

  LitmusSchedulingTraceRecord *litmusSchedulingTraceRecord;
  LitmusInterArrivalTime *litmusInterArrivalTime;
  map<pair<int,int>,LitmusSchedulingTraceRecord*>::iterator it;

  litmusSchedulingTraceRecord = new LitmusInterArrivalTime(ST_COMPLETION);

  it = registeredTraceRecords.begin();
  registeredTraceRecords.insert(it, pair<pair<int,int>,LitmusSchedulingTraceRecord*>
				(pair<int,int>(ST_COMPLETION,ster->hdr.pid),litmusSchedulingTraceRecord));

  // register the release scheduling trace; notice that LitmusSchedulingTraceRecord is the same
  it = registeredTraceRecords.begin();
  registeredTraceRecords.insert(it,pair<pair<int,int>,LitmusSchedulingTraceRecord*>
				(pair<int,int>(ST_RELEASE,ster->hdr.pid),litmusSchedulingTraceRecord));

  litmusSchedulingTraceRecord->setLitmusSchedulingTraceRecordObserver(taskSet);
}

bool SchedulingTraceProcessor::registerSchedulingTrace(struct st_event_record* ster) {

  if (ster->hdr.type ==	ST_RELEASE || ster->hdr.type == ST_COMPLETION) {
    
    registerLitmusExecutionTime(ster);
    registerLitmusInterArrivalTime(ster);
    return true;
  } else {
    
    return false;
  }

}
void SchedulingTraceProcessor::processRegisteredSchedulingTrace(struct st_event_record* ster) {
  
  this->registeredTraceRecords[pair<int,int>(ster->hdr.type, ster->hdr.pid)]->check(ster);
}


void SchedulingTraceProcessor::notifyNewExecutionTime(exec_time_t exec_time, task_id_t task_id) {
  
  if (printExecutionTimes) {
    
    std::cout<<"current execution time for "<<task_id<<" is (NS): \t"<<exec_time<<std::endl;
  }

  taskSet->updateTaskExecCost(exec_time, task_id);
}

void SchedulingTraceProcessor::setPrintSchedulingTraces(bool printSchedulingTraces) {

  this->printSchedulingTraces = printSchedulingTraces;
}

void SchedulingTraceProcessor::setPrintExecutionTimes(bool printExecutionTimes) {

  this->printExecutionTimes = printExecutionTimes;
}
