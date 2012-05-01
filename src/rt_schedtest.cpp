/*    rt_schedtest -- Online overhead aware schedulability testing
 *      
 *    Youcef A. Benabbas under the supervision of Dr. Björn B. Brandenburg,
 */
#include "cmdlparser.h"
#include "taskset.hpp"
#include "overhead.hpp"
#include "schedtest.hpp"
#include "litmusoverhead.hpp"
#include "litmusschedulingtrace.hpp"

#include <cstdlib>
#include <cstdio>


LitmusOverhead *litmusOverhead;
LitmusSchedulingTrace *litmusSchedulingTrace;


void finishTesting(int sig)
{
  litmusOverhead->stopTracing();
  litmusSchedulingTrace->stopTracing();
  exit(0);
}

int main(int argc, char **argv) {

  TaskSet *taskSet;
  Overhead *overhead;
  SchedTest schedTest;  
  CmdlParser cmdlParser(argc, argv);    

  litmusOverhead = LitmusOverhead::getInstance();
  litmusSchedulingTrace = new LitmusSchedulingTrace;

  overhead = Overhead::getInstance();
  taskSet = TaskSet::getInstance();

  signal(SIGINT, finishTesting);
  signal(SIGUSR1, finishTesting);
  signal(SIGTERM, finishTesting);

  schedTest.setTaskSet(taskSet);
  schedTest.setOverhead(overhead);
  overhead->setSchedTestObserver(&schedTest);
  litmusOverhead->setLitmusOverheadObserver(overhead);  

  taskSet->setParameters(cmdlParser);
  overhead->setParameters(cmdlParser);
  litmusOverhead->setParameters(cmdlParser);
  litmusSchedulingTrace->setParameters(cmdlParser);
  
  litmusSchedulingTrace->initDev("/dev/litmus/sched_trace0");
  litmusOverhead->initDev("/dev/litmus/ft_trace0");

  litmusSchedulingTrace->startTracing();

  litmusOverhead->startTracing();
  
  return 0;
}
