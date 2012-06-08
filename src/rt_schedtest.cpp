/*    rt_schedtest -- Online overhead aware schedulability testing
 *      
 *    Youcef A. Benabbas.
 */
#include "cmdlparser.h"
#include "taskset.hpp"
#include "overhead.hpp"
#include "schedtest.hpp"
#include "litmusdaemonlistner.hpp"


int main(int argc, char **argv) {

  TaskSet *taskSet;
  Overhead *overhead;
  SchedTest schedTest;  
  LitmusDaemonListner *litmusDaemonListner;
  CmdlParser cmdlParser(argc, argv);    

  overhead = Overhead::getInstance();
  taskSet = TaskSet::getInstance();
  litmusDaemonListner = LitmusDaemonListner::getInstance();

  schedTest.setTaskSet(taskSet);
  schedTest.setOverhead(overhead);
  overhead->setSchedTestObserver(&schedTest);

  litmusDaemonListner->setOverheadObserver(overhead);
  litmusDaemonListner->setTaskSetObserver(taskSet);

  taskSet->setParameters(cmdlParser);
  overhead->setParameters(cmdlParser);

  litmusDaemonListner->initLitmusDaemonListner();
  
  return 0;
}
