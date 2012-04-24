#include "singlelitmustimestamp.hpp"

SingleLitmusTimestamp::SingleLitmusTimestamp(cmd_t id)
  : LitmusTimestamp(id) {
  this->startID = id;
}

void SingleLitmusTimestamp::check(struct timestamp* ts) {
  if ((ts->event == startID)
      &&(ts->task_type == TSK_RT)) {
    updateLitmusTimestampObservers(ts->timestamp,ts->event);
  }
}
