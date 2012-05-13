/*    rt_schedtest -- Online overhead aware schedulability testing
 *      
 *    Youcef A. Benabbas under the supervision of Dr. Björn B. Brandenburg,
 */
#include "cmdlparser.h"
#include "taskset.hpp"
#include "overhead.hpp"
#include "schedtest.hpp"
#include "litmusdaemon.hpp"


int main(int argc, char **argv) {

  TaskSet *taskSet;
  Overhead *overhead;
  SchedTest schedTest;  
  LitmusDaemon * litmusDaemon;
  CmdlParser cmdlParser(argc, argv);    

  overhead = Overhead::getInstance();
  taskSet = TaskSet::getInstance();
  litmusDaemon = LitmusDaemon::getInstance();

  schedTest.setTaskSet(taskSet);
  schedTest.setOverhead(overhead);
  overhead->setSchedTestObserver(&schedTest);

  litmusDaemon->setOverheadObserver(overhead);
  litmusDaemon->setTaskSetObserver(taskSet);

  taskSet->setParameters(cmdlParser);
  overhead->setParameters(cmdlParser);

  litmusDaemon->forkDaemon();
  
  return 0;
}
