#include "litmusschedulingtracerecord.hpp"


LitmusSchedulingTraceRecord::LitmusSchedulingTraceRecord() {

}

LitmusSchedulingTraceRecord::LitmusSchedulingTraceRecord(ster_t schedulingTraceEventRecordID) {

  this->schedulingTraceEventRecordID = schedulingTraceEventRecordID;
  this->startID = schedulingTraceEventRecordID;
  this->taskSet = TaskSet::getInstance();
}
