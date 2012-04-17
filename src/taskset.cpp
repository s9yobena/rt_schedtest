#include "taskset.hpp"

TaskSet* TaskSet::taskSetInstance = 0;


TaskSet::TaskSet() {
  nb_rts = 0;
}

TaskSet* TaskSet::getInstance() {
  
  if (!taskSetInstance) {
    taskSetInstance = new TaskSet();
  }
  return taskSetInstance;
}

void TaskSet::setParameters(const CmdlParser& cmdlParser) {
  

  this->nb_rts = cmdlParser.nb_rts;
  for (int i=0; i< this->nb_rts; i++) {
    this->rt_task_id[i] = cmdlParser.rt_task_id[i];
    get_rt_task_param(this->rt_task_id[i],&this->rt_task_param[i]);
  }
}

void TaskSet::printParameters() {

  for (int i=0;i<this->nb_rts;i++){
    printf("rt_task %d: e = %d, p = %d \n",this->rt_task_id[i], 
  	   (int)this->rt_task_param[i].exec_cost, 
  	   (int)this->rt_task_param[i].period);
  }
}

 
