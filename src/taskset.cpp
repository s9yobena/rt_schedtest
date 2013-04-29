#include "taskset.hpp"

TaskSet::TaskSet() {
  averageExecCost = 0;
  printExecutionTimes = false;
  printDebug = false;
}

void TaskSet::updateAllTasks(SchedTestParam* schedTestParam) {
  vector<TaskParam>::iterator it;
  for (it = schedTestParam->taskParams.begin(); 
       it != schedTestParam->taskParams.end(); it++) {

    Task *task = new Task();

    task->setId(it->id);
    task->setExecCost(it->e);
    task->setPeriod(it->p);
    task->setSelfSuspension(it->ss);
    task->setCpu(it->cpu);
    task->setPerJobMaxSelfSusp(it->perJobMaxSelfSusp);

    addTask(task);
  }
}

void TaskSet::updateTaskExecCost(lt_t exec_time, pid_t task_id) {

  if (printDebug) {
    
    cout<<"printing task set parameters before updating task "<<task_id<<"exec_cost"<<endl;

      printf("rt_task %d: worst_e = %d, p = %d  sum self-suspension %d\n",
	     task_id, 
	     (int)getTaskExecCost(task_id), 
	     (int)getTaskPeriod(task_id),
	     (int)getTaskSelfSuspension(task_id));
  }
  
  if (isNewTask(task_id))
    addTask(task_id);

  // update task average execution time
  tasks[task_id].updateAvrgExecTime(exec_time);
  if (printExecutionTimes) {
    cout<<"task id:"<<task_id<<" average exec time is "<<tasks[task_id].getAvrgExecTime() <<endl;
  }
  
  // update task worst execution time
  updateMaxExecCost(exec_time, task_id);  

  if (printDebug) {

    cout<<"printing task set parameters after updating task "<<task_id<<"exec_cost"<<endl;

      printf("rt_task %d: worst_e = %d, p = %d  sum self-suspension %d\n",
	     task_id, 
	     (int)getTaskExecCost(task_id), 
	     (int)getTaskPeriod(task_id),
	     (int)getTaskSelfSuspension(task_id));
  }

}

void TaskSet::updateTaskInterArrivalTime(lt_t inter_arrival_time, pid_t task_id) {

  if (!isNewTask(task_id)) {

    updateMinInterArrivalTime(inter_arrival_time, task_id);  
  }
}

void TaskSet::updateTaskSelfSuspension(lt_t self_suspension_time, pid_t task_id) {

  if (!isNewTask(task_id)) {
    
    updatePerJobMaxSelfSusp(self_suspension_time, task_id);
    updateSumSelfSuspension(self_suspension_time, task_id);
  }
}

bool TaskSet::isNewTask(pid_t taskId) {
  
  map<pid_t,Task>::iterator it;
  it = tasks.find(taskId);
  return (it == tasks.end() ? true : false);
}

void TaskSet::addTask(pid_t taskId) {
  
  Task tmpTask;
  struct rt_task tmpTaskParam;
  get_rt_task_param(taskId,&tmpTaskParam); 

  tmpTask.setId(taskId);

  // set exec_cost to 0 to be able to check for the current maximum value
  // even if the user defined one is arbitrarly large
  tmpTaskParam.exec_cost = 0;
  tmpTask.setParameters(tmpTaskParam);

  tmpTask.setSelfSuspension(0);
  tmpTask.setPerJobMaxSelfSusp(0);

  tasks.insert(pair<pid_t,Task>(taskId,tmpTask));
}

void TaskSet::updateMaxExecCost(lt_t exec_time, pid_t taskId) {

  map<pid_t,Task>::iterator it;
  it = tasks.find(taskId);
  if (exec_time > it->second.getExecCost()) {
    it->second.setExecCost(exec_time);
    if (printExecutionTimes)
      printf("rt_task %d: Max. exec_time = %d \n",taskId, 
	     (int)exec_time);
      
  }
}

void TaskSet::updateMinInterArrivalTime(lt_t inter_arrival_time, pid_t taskId) {

  map<pid_t,Task>::iterator it;
  it = tasks.find(taskId);
  if (inter_arrival_time < it->second.getPeriod()) {
    it->second.setPeriod(inter_arrival_time);
    if (printExecutionTimes)
     printf("rt_task %d: Min. inter_arrival_time = %d \n",
	    taskId, 
	    (int)inter_arrival_time);
  }
}

void TaskSet::updateSumSelfSuspension(lt_t self_suspension_time, pid_t taskId) {

  map<pid_t,Task>::iterator it;
  it = tasks.find(taskId);
  it->second.addSelfSuspension(self_suspension_time);
  if (printExecutionTimes)  
    printf("rt_task %d: Sum. Self Suspension = %d \n",
	   taskId, 
	   (int)it->second.getSelfSuspension());
}

void TaskSet::updatePerJobMaxSelfSusp(lt_t self_suspension_time, pid_t taskId) {

  map<pid_t,Task>::iterator it;
  it = tasks.find(taskId);

  if (self_suspension_time > it->second.getPerJobMaxSelfSusp()) {
    it->second.setPerJobMaxSelfSusp(self_suspension_time);
  if (printExecutionTimes)  
    printf("rt_task %d: Max. Self Suspension = %d \n",
	   taskId, 
	   (int)it->second.getPerJobMaxSelfSusp());
  }
}



void TaskSet::setParameters(const CmdlParser& cmdlParser) {
  
  printExecutionTimes = cmdlParser.printExecutionTimes;
}

void TaskSet::printParameters() {

  if (printExecutionTimes)  {
    printf("printing task set parameters \n");
    
    map<pid_t,Task>::iterator it;
    for (it = tasks.begin(); it != tasks.end(); it++) {

      printf("rt_task %d: worst_e = %d, p = %d  sum self-suspension %d\n",
	     it->first, 
	     (int)it->second.getExecCost(), 
	     (int)it->second.getPeriod(),
	     (int)it->second.getSelfSuspension());
    }
  }  
}

int TaskSet::getNbrTasks() {
  return tasks.size();
}

unsigned TaskSet::getTaskCpu(pid_t taskId) {
  return tasks[taskId].getCpu();
}

lt_t TaskSet::getTaskExecCost(pid_t taskId) {
  return tasks[taskId].getExecCost();
}

lt_t TaskSet::getTaskAvrgExecCost(pid_t  taskId) {
  return tasks[taskId].getAvrgExecTime();
}

lt_t TaskSet::getTaskDeadline(pid_t taskId) {
  return tasks[taskId].getDeadline();
}

lt_t TaskSet::getTaskPeriod(pid_t taskId) {
  return tasks[taskId].getPeriod();
}

double TaskSet::getTaskUtilization(pid_t taskId) {
  return tasks[taskId].getUtilization();
}

lt_t TaskSet::getTaskSelfSuspension(pid_t taskId) {
  return tasks[taskId].getSelfSuspension();
}


lt_t TaskSet::getPerJobMaxSelfSusp(pid_t taskId) {
  return tasks[taskId].getPerJobMaxSelfSusp();
}

lt_t TaskSet::getTaskTardiness(pid_t taskId) {
  return tasks[taskId].getTardiness();
}

void TaskSet::setTaskCpu(pid_t taskId, unsigned cpu) {
  tasks[taskId].setCpu(cpu);
}

void TaskSet::setTaskExecCost(pid_t taskId, lt_t execCost) {
  tasks[taskId].setExecCost(execCost);
}

void TaskSet::setTaskPeriod(pid_t taskId, lt_t period) {
  tasks[taskId].setPeriod(period);
}

void TaskSet::setTaskSelfSuspension(pid_t taskId, lt_t selfSusp) {
  tasks[taskId].setSelfSuspension(selfSusp);
}

void TaskSet::addTask(Task *task) {
  tasks.insert(pair<pid_t,Task>(task->getId(),*task));
}

void TaskSet::removeTask(pid_t taskId) {

  if (printDebug) {
    cout<<"printing task set parameters before removing task"<<taskId<<endl;

    map<pid_t,Task>::iterator it;
    for (it = tasks.begin(); it != tasks.end(); it++) {

      printf("rt_task %d: worst_e = %d, p = %d  sum self-suspension %d\n",
	     it->first, 
	     (int)it->second.getExecCost(), 
	     (int)it->second.getPeriod(),
	     (int)it->second.getSelfSuspension());
    }
    cout<<endl;
  }

  tasks.erase(taskId);  

  if (printDebug) {
    cout<<"printing task set parameters after removing task"<<taskId<<endl;

    map<pid_t,Task>::iterator it;
    for (it = tasks.begin(); it != tasks.end(); it++) {

      printf("rt_task %d: worst_e = %d, p = %d  sum self-suspension %d\n",
	     it->first, 
	     (int)it->second.getExecCost(), 
	     (int)it->second.getPeriod(),
	     (int)it->second.getSelfSuspension());
    }
    cout<<endl;
  }
}

Task* TaskSet::getTask(pid_t taskId) {
  return &tasks[taskId];
}

lt_t TaskSet::computeAverageExecCost() {
  map<pid_t,Task>::iterator it;
  lt_t sumExecCost = 0;
  for (it = tasks.begin(); it != tasks.end(); it++) {
    sumExecCost += it->second.getExecCost();
  }  
  return sumExecCost/getNbrTasks();
}

lt_t TaskSet::getAverageExecCost() {
  return averageExecCost;
}

void TaskSet::setAverageExecCost(lt_t _avrgExecCost) {
  averageExecCost = _avrgExecCost;
}

