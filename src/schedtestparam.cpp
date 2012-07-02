#include "schedtestparam.hpp"
#include "taskset.hpp"

SchedTestParam::SchedTestParam() {
  taskSet = TaskSet::getInstance();
}

SchedTestParam* SchedTestParam::schedTestParamInstance = 0;

SchedTestParam* SchedTestParam::getInstance() {
  if (!schedTestParamInstance) {
    schedTestParamInstance = new SchedTestParam();
  }
  return schedTestParamInstance;
}

void SchedTestParam::setOutputName(const char* name) {
  strcpy(this->name, name);
}

char* SchedTestParam::getOutputName() {
  return name;
}

void SchedTestParam::initSchedTestParam() {
  endSchedTestParam = "End";
  resetLocalParams();
}

void SchedTestParam::resetLocalParams() {
  cxs = 0;
  sched = 0;
  sched2 = 0;
  taskParams.clear();
}

void SchedTestParam::setCXS(overhead_t cxs) {
  this->cxs = cxs;
}

void SchedTestParam::setSCHED(overhead_t sched) {
  this->sched = sched;
}

void SchedTestParam::setSCHED2(overhead_t sched2) {
  this->sched2 = sched2;
}

void SchedTestParam::setRELEASE(overhead_t release) {
  this->release = release;
}

void SchedTestParam::setSEND_RESCHED(overhead_t send_resched) {
  this->send_resched = send_resched;
}

void SchedTestParam::setTICK(overhead_t tick) {
  this->tick = tick;
}

void SchedTestParam::setRELEASE_LATENCY(overhead_t release_latency) {
  this->release_latency = release_latency;
}


overhead_t SchedTestParam::getCXS() {
  return cxs;
}

overhead_t SchedTestParam::getSCHED() {
  return sched;
}

overhead_t SchedTestParam::getSCHED2() {
  return sched2;
}

overhead_t SchedTestParam::getRELEASE() {
  return release;
}

overhead_t SchedTestParam::getSEND_RESCHED() {
  return send_resched;
}

overhead_t SchedTestParam::getRELEASE_LATENCY() {
  return release_latency;
}

overhead_t SchedTestParam::getTICK() {
  return tick;
}

vector<TaskParam> SchedTestParam::getTaskParams() {
  return taskParams;
}

void SchedTestParam::addTask(TaskParam taskParam) {
  taskParams.push_back(taskParam);
}

void SchedTestParam::addAllTasks() {
  unsigned taskParamPos = startTaskPos;
  for (int i=0; i<taskSet->getNbrTasks(); i++) {

    pid_t taskId;
    taskId = taskSet->getTaskId(i);

    setTaskParam(TaskParam(taskId,
			   taskSet->getTaskExecCost(taskId),
			   0,
			   taskSet->getTaskPeriod(taskId),
			   taskSet->getTaskSelfSuspension(taskId)),
		 taskParamPos);
    taskParamPos++;
  }
}


void SchedTestParam::getAllTasks() {
  vector<TaskParam>::iterator it;
  unsigned taskParamPos = startTaskPos;
  TaskParam taskParam(0,0,0,0,0);
  int i=0;
  while(!getTaskParam(&taskParam,startTaskPos+i)) {
    taskParams.push_back(taskParam);
    i++;
  }
}

int SchedTestParam::endOfSchedTestParam(const char *line) {
  if(!strcmp(line,endSchedTestParam))
    return 1;
  else
    return 0;
}

void SchedTestParam::setTaskParam(TaskParam taskParam, unsigned pos) {
  rewind(schedTestPramFile);
  char line[100];
  char buf[100];
  unsigned currLineNbr;
  currLineNbr = 0;
  do {
    if (currLineNbr == pos) {
      sprintf(buf, "%u %llu %llu %llu %llu \n", 
	      taskParam.id, taskParam.e, taskParam.d, taskParam.p, taskParam.ss);
      fputs (buf, schedTestPramFile);
      break;
    }
    currLineNbr++;
  } while (fgets (line, sizeof line, schedTestPramFile) != NULL);
}

int SchedTestParam::getTaskParam(TaskParam *taskParam, unsigned pos) {
  rewind(schedTestPramFile);
  char line[100];
  unsigned currLineNbr;
 
  currLineNbr = 0;
  while (fgets (line, sizeof line, schedTestPramFile) != NULL) {
    if (currLineNbr == pos) {
      // check if we reached the end of file
      if (endOfSchedTestParam(line)) {
      	return 1;
      }
      sscanf(line,"%u %u %u %u %u",
	     &taskParam->id, &taskParam->e, &taskParam->d, &taskParam->p, &taskParam->ss);
      return 0;
    }
    currLineNbr++;
  }
  // we should not reach here
  printf("Error parsing file; Correct input file format? \n");
}

void SchedTestParam::setParam(unsigned value, unsigned pos) {
  rewind(schedTestPramFile);
  char line[100];
  char buf[100];
  unsigned currLineNbr;
  currLineNbr = 0;
  do {
    if (currLineNbr == pos) {
      sprintf(buf, "%d\n", value);
      fputs (buf, schedTestPramFile);
      break;
    }
    currLineNbr++;
  } while (fgets (line, sizeof line, schedTestPramFile) != NULL);
}

unsigned SchedTestParam::getParam(unsigned pos) {
  rewind(schedTestPramFile);
  char line[100];
  unsigned currLineNbr;
  unsigned ret;
 
  currLineNbr = 0;
  while (fgets (line, sizeof line, schedTestPramFile) != NULL) {
    if (currLineNbr == pos) {
      sscanf(line,"%u",&ret);
      return ret;
    }
    currLineNbr++;
  }
}

void SchedTestParam::addEndMark() {

  schedTestPramFile = fopen(name,"a");
  fputs(endSchedTestParam,schedTestPramFile);
  fclose(schedTestPramFile); 
}

// Do not modify this function unless you know 
// very well what you are doing
void SchedTestParam::makeSchedTestParam() {
  schedTestPramFile = fopen(name,"w+");
  // DO NOT CHANGE THE ORDER OF THE FOLLOWING BLOCK
  // START
  setParam(cxs, cxsPos);
  setParam(sched, schedPos);
  setParam(sched2, sched2Pos);
  setParam(release, releasePos);
  setParam(send_resched, send_reschedPos);
  setParam(release_latency, release_latencyPos);
  setParam(tick, tickPos);
  addAllTasks();  
  // END
  fclose(schedTestPramFile); 
  addEndMark();
}

void SchedTestParam::getSchedTestParam() {
  schedTestPramFile = fopen(name,"r+");
  if (schedTestPramFile==NULL){
    perror("could not open schedtestparam file");
    exit(EXIT_FAILURE);
  }

  // DO NOT CHANGE THE ORDER OF THE FOLLOWING BLOCK
  // START
  cxs = getParam(cxsPos);
  sched = getParam(schedPos);
  sched2 = getParam(sched2Pos);
  release = getParam(releasePos);
  send_resched = getParam(send_reschedPos);
  release_latency = getParam(release_latencyPos);
  tick = getParam(tickPos);
  getAllTasks();
  // END  
  fclose(schedTestPramFile); 
}

void SchedTestParam::setParameters(const CmdlParser& cmdlParser) {
  
  strcpy(name, cmdlParser.stfName);
}
