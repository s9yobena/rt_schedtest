#include "singlelitmustimestamp.hpp"

SingleLitmusTimestamp::SingleLitmusTimestamp(cmd_t id)
  : LitmusTimestamp(id) {
  this->startID = id;
}

void SingleLitmusTimestamp::check(struct timestamp* ts) {
  if ((ts->event == startID)
      &&(ts->task_type == TSK_RT)) {

    std::cout<<"current overhead for "<<this->startID<<" is: \t"<<ts->timestamp<<std::endl;
    updateLitmusTimestampObservers(ts->timestamp,ts->event);
  }
}
