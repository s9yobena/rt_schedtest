#include "taskset.hpp"

TaskSet::TaskSet() {
  averageExecCost = 0;
  printExecutionTimes = false;
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
  
  if (isNewTask(task_id))
    addTask(task_id);

  // update task average execution time
  taskSet[task_id].updateAvrgExecTime(exec_time);
  if (printExecutionTimes) {
    cout<<"task id:"<<task_id<<" average exec time is "<<taskSet[task_id].getAvrgExecTime() <<endl;
  }
  
  // update task worst execution time
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
  
  updatePerJobMaxSelfSusp(self_suspension_time, task_id);
  updateSumSelfSuspension(self_suspension_time, task_id);
}

bool TaskSet::isNewTask(pid_t taskId) {
  
  for (int i=0; i< getNbrTasks(); i++){
    if (taskId == tasksId[i]) {
      return false;
    }
  }
  return true;
}

void TaskSet::addTask(pid_t taskId) {
  
  tasksId.push_back(taskId);
  
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

  taskSet.insert(pair<pid_t,Task>(taskId,tmpTask));
}

void TaskSet::updateMaxExecCost(lt_t exec_time, pid_t taskId) {

  map<pid_t,Task>::iterator it;
  it = taskSet.find(taskId);
  if (exec_time > it->second.getExecCost()) {
    it->second.setExecCost(exec_time);
    if (printExecutionTimes)
      printf("rt_task %d: Max. exec_time = %d \n",taskId, 
	     (int)exec_time);
      
  }
}

void TaskSet::updateMinInterArrivalTime(lt_t inter_arrival_time, pid_t taskId) {

  map<pid_t,Task>::iterator it;
  it = taskSet.find(taskId);
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
  it = taskSet.find(taskId);
  it->second.addSelfSuspension(self_suspension_time);
  if (printExecutionTimes)  
    printf("rt_task %d: Sum. Self Suspension = %d \n",
	   taskId, 
	   (int)it->second.getSelfSuspension());
}

void TaskSet::updatePerJobMaxSelfSusp(lt_t self_suspension_time, pid_t taskId) {

  map<pid_t,Task>::iterator it;
  it = taskSet.find(taskId);

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
    for (int i=0;i<getNbrTasks();i++){

      pid_t taskId;
      taskId = getTaskId(i);

      printf("rt_task %d: worst_e = %d, p = %d  sum self-suspension %d\n",
	     taskId, 
	     (int)getTaskExecCost(taskId), 
	     (int)getTaskPeriod(taskId),
	     (int)getTaskSelfSuspension(taskId));
    }
  }  
}

int TaskSet::getNbrTasks() {
  return taskSet.size();
}

pid_t TaskSet::getTaskId(int i) {
  return tasksId[i];
}

unsigned TaskSet::getTaskCpu(pid_t taskId) {
  return taskSet[taskId].getCpu();
}

lt_t TaskSet::getTaskExecCost(pid_t taskId) {
  return taskSet[taskId].getExecCost();
}

lt_t TaskSet::getTaskAvrgExecCost(pid_t  taskId) {
  return taskSet[taskId].getAvrgExecTime();
}

lt_t TaskSet::getTaskDeadline(pid_t taskId) {
  return taskSet[taskId].getDeadline();
}

lt_t TaskSet::getTaskPeriod(pid_t taskId) {
  return taskSet[taskId].getPeriod();
}

double TaskSet::getTaskUtilization(pid_t taskId) {
  return taskSet[taskId].getUtilization();
}

lt_t TaskSet::getTaskSelfSuspension(pid_t taskId) {
  return taskSet[taskId].getSelfSuspension();
}


lt_t TaskSet::getPerJobMaxSelfSusp(pid_t taskId) {
  return taskSet[taskId].getPerJobMaxSelfSusp();
}

lt_t TaskSet::getTaskTardiness(pid_t taskId) {
  return taskSet[taskId].getTardiness();
}

void TaskSet::setTaskCpu(pid_t taskId, unsigned cpu) {
  taskSet[taskId].setCpu(cpu);
}

void TaskSet::setTaskExecCost(pid_t taskId, lt_t execCost) {
  taskSet[taskId].setExecCost(execCost);
}

void TaskSet::setTaskPeriod(pid_t taskId, lt_t period) {
  taskSet[taskId].setPeriod(period);
}

void TaskSet::setTaskSelfSuspension(pid_t taskId, lt_t selfSusp) {
  taskSet[taskId].setSelfSuspension(selfSusp);
}

void TaskSet::addTask(Task *task) {
  tasksId.push_back(task->getId());
  taskSet.insert(pair<pid_t,Task>(task->getId(),*task));
}

void TaskSet::removeTask(pid_t taskId) {
  for (int i=0; i< tasksId.size(); i++) {
    if (tasksId[i] == taskId) {
      tasksId.erase(tasksId.begin()+i);
      break;
    }
  }
  taskSet.erase(taskId);  
}

Task* TaskSet::getTask(pid_t taskId) {
  return &taskSet[taskId];
}

lt_t TaskSet::computeAverageExecCost() {
  map<pid_t,Task>::iterator it;
  lt_t sumExecCost = 0;
  for (it = taskSet.begin(); it != taskSet.end(); it++) {
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

