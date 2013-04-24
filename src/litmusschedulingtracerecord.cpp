#include "litmusschedulingtracerecord.hpp"

LitmusSchedulingTraceRecord::LitmusSchedulingTraceRecord() {

}

LitmusSchedulingTraceRecord::LitmusSchedulingTraceRecord(ster_t schedulingTraceEventRecordID) {

  this->schedulingTraceEventRecordID = schedulingTraceEventRecordID;
  this->startID = schedulingTraceEventRecordID;

  schedTestParam = SchedTestParam::getInstance();
  litmusSchedTest = LitmusSchedTest::getInstance();
}

void LitmusSchedulingTraceRecord::setTaskSet(TaskSet *_taskSet) {
  taskSet = _taskSet;
}

