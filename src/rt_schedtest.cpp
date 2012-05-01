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
LitmusSchedulingTrace *litmusSchedulingTrace0;
LitmusSchedulingTrace *litmusSchedulingTrace1;


void finishTesting(int sig)
{
  litmusOverhead->stopTracing();
  litmusSchedulingTrace0->stopTracing();
  litmusSchedulingTrace1->stopTracing();
  exit(0);
}

int main(int argc, char **argv) {

  TaskSet *taskSet;
  Overhead *overhead;
  SchedTest schedTest;  
  CmdlParser cmdlParser(argc, argv);    

  litmusOverhead = LitmusOverhead::getInstance();
  litmusSchedulingTrace0 = new LitmusSchedulingTrace;
  litmusSchedulingTrace1 = new LitmusSchedulingTrace;

  overhead = Overhead::getInstance();
  taskSet = TaskSet::getInstance();

  signal(SIGINT, finishTesting);
  signal(SIGUSR1, finishTesting);
  signal(SIGTERM, finishTesting);

  schedTest.setTaskSet(taskSet);
  schedTest.setOverhead(overhead);
  overhead->setSchedTestObserver(&schedTest);

  taskSet->setParameters(cmdlParser);
  overhead->setParameters(cmdlParser);
  litmusOverhead->setParameters(cmdlParser);
  litmusSchedulingTrace0->setParameters(cmdlParser);
  litmusSchedulingTrace1->setParameters(cmdlParser);
  
  litmusSchedulingTrace0->initDev("/dev/litmus/sched_trace0");
  litmusSchedulingTrace1->initDev("/dev/litmus/sched_trace1");
  litmusOverhead->initDev("/dev/litmus/ft_trace0");

  litmusSchedulingTrace0->startTracing();
  litmusSchedulingTrace1->startTracing();
  litmusOverhead->startTracing();
  
  return 0;
}
