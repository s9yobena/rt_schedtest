/*    rt_global-test
 *      
 *    Youcef A. Benabbas.
 */
#include "cmdlparser.h"
#include "taskset.hpp"
#include "overhead.hpp"
#include "globaltest.hpp"
#include "schedtestparam.hpp"


int main(int argc, char **argv) {

  TaskSet *taskSet;
  Overhead *overhead;
  GlobalTest globalTest;
  SchedTestParam *schedTestParam;
  CmdlParser cmdlParser(argc, argv);    

  overhead = Overhead::getInstance();
  taskSet = new TaskSet();
  schedTestParam = SchedTestParam::getInstance();

  globalTest.setTaskSet(taskSet);
  globalTest.setOverhead(overhead);

  schedTestParam->initSchedTestParam();
  schedTestParam->setParameters(cmdlParser);
  schedTestParam->getSchedTestParam();

  overhead->updateAllOverheads(schedTestParam);
  
  taskSet->updateAllTasks(schedTestParam);

  taskSet->setParameters(cmdlParser);
  overhead->setParameters(cmdlParser);

  globalTest.setMHzCpuClock(schedTestParam->getMHzCpuClock());
  globalTest.setNbrCpus(schedTestParam->getNbrCpus());
  globalTest.makeSchedTest();

  return 0;
}
