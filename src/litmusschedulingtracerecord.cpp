#include "litmusschedulingtracerecord.hpp"


LitmusSchedulingTraceRecord::LitmusSchedulingTraceRecord() {

}

LitmusSchedulingTraceRecord::LitmusSchedulingTraceRecord(ster_t schedulingTraceEventRecordID) {

  this->schedulingTraceEventRecordID = schedulingTraceEventRecordID;
  this->startID = schedulingTraceEventRecordID;
  this->taskSet = TaskSet::getInstance();

  schedTestParam = SchedTestParam::getInstance();
  litmusSchedTest = LitmusSchedTest::getInstance();
  litmusSchedTest->setDensityTest();

}
