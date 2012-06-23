#include "schedulingtraceprocessor.hpp"

SchedulingTraceProcessor::SchedulingTraceProcessor(){
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

  if (!isRegisteredSchedulingTrace(ster))
    registerSchedulingTrace(ster);

  processRegisteredSchedulingTrace(ster);

}

bool SchedulingTraceProcessor::isRegisteredSchedulingTrace(struct st_event_record* ster) {

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
}

void SchedulingTraceProcessor::registerLitmusInterArrivalTime(struct st_event_record* ster) {

  LitmusSchedulingTraceRecord *litmusSchedulingTraceRecord;
  LitmusInterArrivalTime *litmusInterArrivalTime;
  map<pair<int,int>,LitmusSchedulingTraceRecord*>::iterator it;

  litmusSchedulingTraceRecord = new LitmusInterArrivalTime(ST_RELEASE);

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
  LitmusSelfSuspension *litmusSelfSuspension;
  map<pair<int,int>,LitmusSchedulingTraceRecord*>::iterator it;

  litmusSchedulingTraceRecord = new LitmusSelfSuspension(ST_BLOCK);

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
  } else {
    
    return false;
  }

}
void SchedulingTraceProcessor::processRegisteredSchedulingTrace(struct st_event_record* ster) {
  if (ster->hdr.type ==	ST_RELEASE || ster->hdr.type == ST_COMPLETION) {
 
    this->registeredTraceRecords[pair<int,int>(ster->hdr.type, ster->hdr.pid)]->check(ster);
    this->registeredInterArrivalTimeTraceRocords[pair<int,int>(ster->hdr.type, ster->hdr.pid)]->check(ster);
  
  } else if (ster->hdr.type ==	ST_BLOCK || ster->hdr.type == ST_RESUME) {

    this->registeredSelfSuspensions[pair<int,int>(ster->hdr.type, ster->hdr.pid)]->check(ster);

  } else if (ster->hdr.type ==	ST_TERMINATION) {
    
    LitmusTaskTermination *litmusTaskTermination;
    litmusTaskTermination = new LitmusTaskTermination(ST_RELEASE);
    litmusTaskTermination->check(ster);
    delete litmusTaskTermination;
  }
}

void SchedulingTraceProcessor::setPrintSchedulingTraces(bool printSchedulingTraces) {

  this->printSchedulingTraces = printSchedulingTraces;
}

void SchedulingTraceProcessor::setPrintExecutionTimes(bool printExecutionTimes) {

  this->printExecutionTimes = printExecutionTimes;
}
