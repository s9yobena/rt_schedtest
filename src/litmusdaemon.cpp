#include "litmusdaemon.hpp"

LitmusDaemon::LitmusDaemon() {
}

LitmusDaemon* LitmusDaemon::litmusDaemonInstance = 0;

LitmusDaemon* LitmusDaemon::getInstance() {
  
  if (!litmusDaemonInstance) {
    litmusDaemonInstance = new LitmusDaemon();
  }
  return litmusDaemonInstance;
}

void LitmusDaemon::forkDaemon() {

  initLitmusDaemonPipe();

  pid_t pid;
  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  // daemon
  if (pid == 0) {
    initDaemon();
  } 
  // parent
  else {
    initParent();
  }
  
}

void LitmusDaemon::initLitmusDaemonPipe() {

  if (pipe(litmusDaemonPipe) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
}

void LitmusDaemon::initDaemon() {
  
  close(litmusDaemonPipe[0]);		// close read end of pipe
  dup2(litmusDaemonPipe[1], STDOUT_FILENO);
  execv( "./rt_trace-daemon", NULL );
  perror( "execv()" );
  exit(255);
}

void LitmusDaemon::initParent() {

  close(litmusDaemonPipe[1]);		// close write end of pipe

  int rd;
  size_t size, count;
  struct LitmusTracingPacket *ltp, *end;

  while ((rd = read(litmusDaemonPipe[0], litmusDaemonBuffer, DEAMON_BUFFER_SIZE)) > 0) {
      
    size = rd;
    ltp = (struct LitmusTracingPacket*) litmusDaemonBuffer;
    count = size / sizeof(struct LitmusTracingPacket);
    end = ltp + count;
    
    for (; ltp != end; ltp++){
      
      processPacket(ltp);
    }
  }
  close(litmusDaemonPipe[0]);          
  wait(NULL);                /* Wait for child */
  exit(EXIT_SUCCESS);
}

void LitmusDaemon::setOverheadObserver(Overhead *overhead) {

  this->overhead = overhead;
}

void LitmusDaemon::setTaskSetObserver(TaskSet *taskSet) {

  this->taskSet = taskSet;
}

void LitmusDaemon::processPacket(struct LitmusTracingPacket* litmusTracingPacket) {

  switch (litmusTracingPacket->id) {

  case LitmusCXSDataID:
    processLitmusCXSData(litmusTracingPacket->data.litmusCXSData.overhead);
    break;
  case LitmusSCHEDDataID:
    processLitmusSCHEDData(litmusTracingPacket->data.litmusSCHEDData.overhead);
    break;
  case LitmusSCHED2DataID:
    processLitmusSCHED2Data(litmusTracingPacket->data.litmusSCHED2Data.overhead);
    break;
  case LitmusRELEASEDataID:
    processLitmusRELEASEData(litmusTracingPacket->data.litmusRELEASEData.overhead);
    break;
  case LitmusRELEASE_LATENCYDataID:
    processLitmusRELEASE_LATENCYData(litmusTracingPacket->data.litmusRELEASE_LATENCYData.overhead);
    break;
  case LitmusSEND_RESCHEDDataID:
    processLitmusSEND_RESCHEDData(litmusTracingPacket->data.litmusSEND_RESCHEDData.overhead);
    break;
  case LitmusExecutionCostDataID:
    processLitmusExecutionCostData(litmusTracingPacket->data.litmusExecutionCostData.exec_time,
				   litmusTracingPacket->data.litmusExecutionCostData.task_id);
    break;
  case LitmusInterArrivalTimeDataID:
    processLitmusInterArrivalTimeData(litmusTracingPacket->data.litmusInterArrivalTimeData.exec_time,
				      litmusTracingPacket->data.litmusInterArrivalTimeData.task_id);
    break;
  }
  
}

void LitmusDaemon::processLitmusCXSData(overhead_t ovrhd) {

  overhead->updateCXS(ovrhd);
  overhead->updateSchedTest();
}

void LitmusDaemon::processLitmusSCHEDData(overhead_t ovrhd) {

  overhead->updateSCHED(ovrhd);
  overhead->updateSchedTest();
}

void LitmusDaemon::processLitmusSCHED2Data(overhead_t ovrhd) {

  overhead->updateSCHED2(ovrhd);
  overhead->updateSchedTest();
}

void LitmusDaemon::processLitmusRELEASEData(overhead_t ovrhd) {

  overhead->updateRELEASE(ovrhd);
  overhead->updateSchedTest();
}

void LitmusDaemon::processLitmusRELEASE_LATENCYData(overhead_t ovrhd) {

  overhead->updateRELEASE_LATENCY(ovrhd);
  overhead->updateSchedTest();
}

void LitmusDaemon::processLitmusSEND_RESCHEDData(overhead_t ovrhd) {

  overhead->updateSEND_RESCHED(ovrhd);
  overhead->updateSchedTest();
}

void LitmusDaemon::processLitmusExecutionCostData(exec_time_t exec_time, task_id_t task_id) {
  
  taskSet->updateTaskExecCost(exec_time,  task_id);
}

void LitmusDaemon::processLitmusInterArrivalTimeData(exec_time_t inter_arrival_time, task_id_t task_id) {

  taskSet->updateTaskInterArrivalTime(inter_arrival_time,  task_id);
}

