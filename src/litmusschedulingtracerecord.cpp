#include "litmusschedulingtracerecord.hpp"


LitmusSchedulingTraceRecord::LitmusSchedulingTraceRecord() {
}

LitmusSchedulingTraceRecord::LitmusSchedulingTraceRecord(ster_t schedulingTraceEventRecordID) {
  this->schedulingTraceEventRecordID = schedulingTraceEventRecordID;
  this->startID = schedulingTraceEventRecordID;
}

void LitmusSchedulingTraceRecord::setLitmusSchedulingTraceRecordObserver(SchedulingTraceProcessor* schedulingTraceProcessor) {
  this->schedulingTraceProcessor = schedulingTraceProcessor;
}

void LitmusSchedulingTraceRecord::updateLitmusSchedulingTraceRecordObservers(exec_time_t exec_time, task_id_t task_id) {
  this->schedulingTraceProcessor->notifyNewExecutionTime(exec_time,task_id);
}
