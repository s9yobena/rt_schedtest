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

void TaskSet::updateAllTasks(SchedTestParam* schedTestParam) {
  vector<TaskParam>::iterator it;
  for (it = schedTestParam->taskParams.begin(); 
       it != schedTestParam->taskParams.end(); it++) {
    updateTaskExecCost(it->e, it->id);
    updateTaskInterArrivalTime(it->p, it->id);
    updateTaskSelfSuspension(it->ss, it->id);
  }

}

void TaskSet::updateTaskExecCost(lt_t exec_time, pid_t task_id) {
  
  if (isNewTask(task_id))
    addTask(task_id);

  updateMaxExecCost(exec_time, task_id);  
}

void TaskSet::updateTaskInterArrivalTime(lt_t inter_arrival_time, pid_t task_id) {

  if (isNewTask(task_id))
    addTask(task_id);

  updateMinInterArrivalTime(inter_arrival_time, task_id);  
}

void TaskSet::updateTaskSelfSuspension(lt_t self_suspension_time, pid_t task_id) {

  if (isNewTask(task_id))
    addTask(task_id);

  updateSumSelfSuspension(self_suspension_time, task_id);
}

bool TaskSet::isNewTask(pid_t task_id) {
  
  for (int i=0; i< nb_rts; i++){
    if (task_id == rt_task_id[i]) {
      return false;
    }
  }
  return true;
}

void TaskSet::addTask(pid_t task_id) {
  
  rt_task_id[nb_rts] = task_id;
  get_rt_task_param(rt_task_id[nb_rts],&rt_task_param[nb_rts]); 

  // set exec_cost to 0 to be able to check for the current maximum value
  // even if the user defined one is arbitrarly large
  rt_task_param[nb_rts].exec_cost = 0;
  rt_task_SelfSuspension[nb_rts] = 0;

  nb_rts++;
}

void TaskSet::updateMaxExecCost(lt_t exec_time, pid_t task_id) {

  for (int i=0; i< nb_rts; i++){
    if (task_id == rt_task_id[i] && exec_time > rt_task_param[i].exec_cost ) {
      
      rt_task_param[i].exec_cost = exec_time;
      if (printExecutionTimes) 
	printf("rt_task %d: Max. exec_time = %d \n",this->rt_task_id[i], 
	       (int)this->rt_task_param[i].exec_cost);
    }
  }
}

void TaskSet::updateMinInterArrivalTime(lt_t inter_arrival_time, pid_t task_id) {
  for (int i=0; i< nb_rts; i++){
    // IMPORTANT: it is assumed that period cannot be arbitrarly small. i.e litmus will
    // not execute the corresponding task.
    if (task_id == rt_task_id[i] && inter_arrival_time < rt_task_param[i].period ) {
      
      rt_task_param[i].period = inter_arrival_time;
      if (printExecutionTimes) 
	printf("rt_task %d: Min. inter_arrival_time = %d \n",this->rt_task_id[i], 
	       (int)this->rt_task_param[i].period);
    }
  }
}

void TaskSet::updateSumSelfSuspension(lt_t self_suspension_time, pid_t task_id) {

  for (int i=0; i< nb_rts; i++){
    if (task_id == rt_task_id[i]) {
      
      rt_task_SelfSuspension[i] += self_suspension_time;
      // if (printExecutionTimes)  
	printf("rt_task %d: Sum. Self Suspension = %d \n",this->rt_task_id[i], 
	       (int)this->rt_task_SelfSuspension[i]);
    }
  }
}

void TaskSet::setParameters(const CmdlParser& cmdlParser) {
  
  printExecutionTimes = cmdlParser.printExecutionTimes;
}

void TaskSet::printParameters() {
  
  printf("printing task set parameters \n");
  for (int i=0;i<this->nb_rts;i++){
    printf("rt_task %d: e = %d, p = %d  sum self-suspension %d\n",this->rt_task_id[i], 
  	   (int)this->rt_task_param[i].exec_cost, 
  	   (int)this->rt_task_param[i].period,
	   (int)this->rt_task_SelfSuspension[i]);
    
  }
}
