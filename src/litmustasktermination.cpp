#include "litmustasktermination.hpp"

LitmusTaskTermination::LitmusTaskTermination(ster_t sterType) 
  : LitmusSchedulingTraceRecord(sterType) {

  schedTestParam = SchedTestParam::getInstance();
  litmusSchedTest = LitmusSchedTest::getInstance();
  litmusSchedTest->setDensityTest();
}

void LitmusTaskTermination::check(struct st_event_record* ster) {
  updateTaskSet(ster->data.termination.when, ster->hdr.pid);
}

void LitmusTaskTermination::updateTaskSet(lt_t termination, pid_t task_id) {
  taskSet->removeTask(task_id);
  schedTestParam->makeSchedTestParam();
  litmusSchedTest->callSchedTest(schedTestParam->getOutputName());
    
}
