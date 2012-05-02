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

void TaskSet::updateTaskExecCost(exec_time_t exec_time, task_id_t task_id) {
  
  if (isNewTask(task_id))
    addTask(task_id);

  updateMaxExecCost(exec_time, task_id);  
}

bool TaskSet::isNewTask(task_id_t task_id) {
  
  for (int i=0; i< nb_rts; i++){
    if (task_id == rt_task_id[i]) {
      return false;
    }
  }
  return true;
}

void TaskSet::addTask(task_id_t task_id) {
  
  rt_task_id[nb_rts] = task_id;
  get_rt_task_param(rt_task_id[nb_rts],&rt_task_param[nb_rts]); 

  // set exec_cost to 0 to be able to check for the current maximum value
  // even if the user defined one is arbitrarly large
  rt_task_param[nb_rts].exec_cost = 0;

  nb_rts++;
}

void TaskSet::updateMaxExecCost(exec_time_t exec_time, task_id_t task_id) {

  for (int i=0; i< nb_rts; i++){
    if (task_id == rt_task_id[i] && exec_time > rt_task_param[i].exec_cost ) {
      
      rt_task_param[i].exec_cost = exec_time;
      if (printExecutionTimes) 
	printf("rt_task %d: Max. exec_time = %d \n",this->rt_task_id[i], 
	       (int)this->rt_task_param[i].exec_cost);
    }
  }
}

void TaskSet::setParameters(const CmdlParser& cmdlParser) {
  
  printExecutionTimes = cmdlParser.printExecutionTimes;
}

void TaskSet::printParameters() {
  
  printf("printing task set parameters \n");
  for (int i=0;i<this->nb_rts;i++){
    printf("rt_task %d: e = %d, p = %d \n",this->rt_task_id[i], 
  	   (int)this->rt_task_param[i].exec_cost, 
  	   (int)this->rt_task_param[i].period);
  }
}
