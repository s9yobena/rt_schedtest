#include "schedtestparam.hpp"
#include "taskset.hpp"
#include "cachetop.hpp"

SchedTestParam::SchedTestParam() {
}

SchedTestParam* SchedTestParam::schedTestParamInstance = 0;

SchedTestParam* SchedTestParam::getInstance() {
  if (!schedTestParamInstance) {
    schedTestParamInstance = new SchedTestParam();
  }
  return schedTestParamInstance;
}

void SchedTestParam::initOutputName() {
  ssname.str("schedtestfile.stf");
  suffI = 0;
  maxSTFiles = 10E6;
}

void SchedTestParam::setOutputName(const char *_name) {
  strcpy(name,_name);
}


void SchedTestParam::generateOutputName() {
  // Generate a new suffix for STF.
  suffI = (suffI +1) % maxSTFiles;
  // Form final STF name.
  ssname.str("");
  ssname<<"schedtestfile"<<suffI<<".stf";
  strcpy(name,ssname.str().c_str());
}


char* SchedTestParam::getOutputName() {
  return name;
}

void SchedTestParam::setTaskSet(TaskSet *_taskSet) {
  taskSet = _taskSet;
}

void SchedTestParam::initSchedTestParam() {
  endSchedTestParam = "End";
  resetLocalParams();
}

void SchedTestParam::resetLocalParams() {

  mhzCpuClock = 0;
  cache_top.clear();

  cxs = 0;
  sched = 0;
  sched2 = 0;
  release = 0;
  send_resched = 0;
  release_latency = 0;
  tick = 0;

  taskParams.clear();
}

void SchedTestParam::setMHzCpuClock(unsigned _mhzCpuClock) {
  mhzCpuClock = _mhzCpuClock;
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

unsigned SchedTestParam::getNbrCpus() {
  unsigned nbrcpus = 0;

  std::vector< std::vector<unsigned> >::const_iterator row;
  std::vector<unsigned>::const_iterator col;
  for (row = cache_top.begin(); row != cache_top.end(); ++row) {
    for (col = row->begin(); col != row->end(); ++col) {
      nbrcpus++;
    }
  }

  return nbrcpus;
}

unsigned SchedTestParam::getMHzCpuClock() {
  return mhzCpuClock;
}

vector<vector<unsigned> > SchedTestParam::getCacheTop() {
  return cache_top;
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

  map<pid_t,Task>::iterator it;
  for (it = taskSet->tasks.begin(); it != taskSet->tasks.end(); it++) {

    setTaskParam(TaskParam(it->first,
			   it->second.getCpu(),
			   it->second.getExecCost(), 
			   0,
			   it->second.getPeriod(),
			   it->second.getSelfSuspension(),
			   it->second.getPerJobMaxSelfSusp()),
		 taskParamPos);
    taskParamPos++;
  }
}


void SchedTestParam::getAllTasks() {
  vector<TaskParam>::iterator it;
  TaskParam taskParam(0,0,0,0,0,0,0);
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

void SchedTestParam::setTaskParam(const TaskParam &taskParam, unsigned pos) {
  rewind(schedTestPramFile);
  char line[100];
  // stringstream buf;
  buf.str("");
  buf.clear();
  unsigned currLineNbr;
  currLineNbr = 0;
  do {
    if (currLineNbr == pos) {

      usleep(500);
      buf<<taskParam.id<<" "<<taskParam.cpu<<" "<<taskParam.e<<" "
	 <<taskParam.d<<" "<<taskParam.p<<" "<<taskParam.ss<<" "
	 <<taskParam.perJobMaxSelfSusp
	 <<endl;
      usleep(500);

      fputs (buf.str().c_str(), schedTestPramFile);
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
      sscanf(line,"%d %u %llu %llu %llu %llu %llu",
	     &taskParam->id, &taskParam->cpu, &taskParam->e, &taskParam->d, &taskParam->p, 
	     &taskParam->ss, &taskParam->perJobMaxSelfSusp);
      return 0;
    }
    currLineNbr++;
  }
  // we should not reach here
  printf("Error parsing file schedtestfile; Correct input file format? \n");
  cout<<"We should not reach here, error"<<endl;
  exit(1);

}

void SchedTestParam::initCacheTopParam() {
  
  cacheTop = new CacheTop();
  cacheTop->drawCacheTop();
  _cacheTop = cacheTop->getCacheTop();
  cacheTopBuf[0] = '\0';
  for (unsigned i=0; i<_cacheTop.size(); i++) {
	
    for (unsigned j=0; j<_cacheTop[i].size(); j++) {

      stringstream tmpB;
      tmpB.str("");
      tmpB<<_cacheTop[i][j]<<",";
      strncat(cacheTopBuf, tmpB.str().c_str(), strlen(tmpB.str().c_str()));
    }
    strcat(cacheTopBuf,"-");
  }
  strcat(cacheTopBuf,"\n");
}


void SchedTestParam::setCacheTopParam(unsigned _cacheTopPos) {

  rewind(schedTestPramFile);
  char line[100];
  unsigned currLineNbr;
  currLineNbr = 0;
  do {
    if (currLineNbr == _cacheTopPos) {
      fputs (cacheTopBuf, schedTestPramFile);
      break;
    }
    currLineNbr++;
  } while (fgets (line, sizeof line, schedTestPramFile) != NULL);

}

vector<vector<unsigned> > SchedTestParam::getCacheTopParam(unsigned cacheTopPos) {
  rewind(schedTestPramFile);
  char line[100];
  unsigned currLineNbr;
  vector<vector<unsigned> > ret;
  vector<unsigned> tmp;
 
  currLineNbr = 0;
  while (fgets (line, sizeof line, schedTestPramFile) != NULL) {
    if (currLineNbr == cacheTopPos) {
      char *str1,*str2,*token,*subtoken;
      char *saveptr1, *saveptr2;
      int cpu;

      for (str1 = line; ; str1 = NULL) {
	token = strtok_r(str1, "-", &saveptr1);
	if (token == NULL ||strcmp(token,"\n")==0 )
	  break;
	tmp.clear();

	for (str2 = token; ; str2 = NULL) {
	  subtoken = strtok_r(str2, ",", &saveptr2);
	  if (subtoken == NULL)
	    break;
	  sscanf(subtoken,"%d,",&cpu);
	  tmp.push_back(cpu);
	}
	  ret.push_back(tmp);
      }
      
      return ret;
    }
    currLineNbr++;
  }
  cout<<"We should not reach here in SchedTestParam::getCacheTopParam"<<endl;
  exit(1);

}

void SchedTestParam::setParam(unsigned value, unsigned pos) {
  rewind(schedTestPramFile);
  char line[100];
  // stringstream buf;
  unsigned currLineNbr;
  currLineNbr = 0;
  buf.str("");
  buf.clear();
  do {
    if (currLineNbr == pos) {
      try {
	
	usleep(500);
	buf<<value
	   <<endl;
	usleep(500);
      } catch (...) {
	cout<<"did throw an exception"
	    <<endl;
      }
      fputs (buf.str().c_str(), schedTestPramFile);
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

  cout<<"We should not reach here SchedTestParam::getParam"<<endl;
  exit(1);
    
}

void SchedTestParam::addEndMark() {

  schedTestPramFile = fopen(name,"a");
  if (schedTestPramFile==NULL){
    perror("could not open schedtestparam file");
    exit(EXIT_FAILURE);
  }
  fputs(endSchedTestParam,schedTestPramFile);
  fclose(schedTestPramFile); 
}

void SchedTestParam::writeSchedTestParams() {
  // DO NOT CHANGE THE ORDER OF THE FOLLOWING BLOCK
  // START
  setParam(mhzCpuClock, cpuClockPos);
  setCacheTopParam(cacheTopPos);
  setParam(cxs, cxsPos);
  setParam(sched, schedPos);
  setParam(sched2, sched2Pos);
  setParam(release, releasePos);
  setParam(send_resched, send_reschedPos);
  setParam(release_latency, release_latencyPos);
  setParam(tick, tickPos);
  addAllTasks();  
  // END
}


// Do not modify this function unless you know 
// very well what you are doing
void SchedTestParam::makeSchedTestParam() {
  // This schedtestfile is kept for reference, as the previous
  // one is consumed (deleted) by the schedulability tests.
  setOutputName("schedtestfile.stf");
  schedTestPramFile = fopen(name,"w+");
  if (schedTestPramFile==NULL){
    perror("could not open schedtestparam file");
    exit(EXIT_FAILURE);
  }
  writeSchedTestParams();
  fclose(schedTestPramFile); 
  addEndMark();

  generateOutputName();
  schedTestPramFile = fopen(name,"w+");
  if (schedTestPramFile==NULL){
    perror("could not open schedtestparam file");
    exit(EXIT_FAILURE);
  }
  writeSchedTestParams();
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
  mhzCpuClock = getParam(cpuClockPos);
  cache_top = getCacheTopParam(cacheTopPos);
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
  deleteSTFile();
}

void SchedTestParam::deleteSTFile() {
  if( remove(name) != 0 )     
    perror( "Error deleting schedtestfile" );
}

void SchedTestParam::setParameters(const CmdlParser& cmdlParser) {
  
  strcpy(name, cmdlParser.stfName);
}
