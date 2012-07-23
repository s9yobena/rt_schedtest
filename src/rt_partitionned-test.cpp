/*    rt_partitionned-test
 *      
 *    Youcef A. Benabbas.
 */
#include "cmdlparser.h"
#include "taskset.hpp"
#include "overhead.hpp"
#include "partitionnedtest.hpp"
#include "schedtestparam.hpp"


int main(int argc, char **argv) {

  TaskSet *taskSet;
  Overhead *overhead;
  PartitionnedTest partitionnedTest;  
  SchedTestParam *schedTestParam;
  CmdlParser cmdlParser(argc, argv);    

  overhead = Overhead::getInstance();
  taskSet = new TaskSet();
  schedTestParam = SchedTestParam::getInstance();

  partitionnedTest.setTaskSet(taskSet);
  partitionnedTest.setOverhead(overhead);

  schedTestParam->initSchedTestParam();
  schedTestParam->setParameters(cmdlParser);
  schedTestParam->getSchedTestParam();

  overhead->updateAllOverheads(schedTestParam);
  
  taskSet->updateAllTasks(schedTestParam);

  taskSet->setParameters(cmdlParser);
  overhead->setParameters(cmdlParser);

  partitionnedTest.setMHzCpuClock(schedTestParam->getMHzCpuClock());
  partitionnedTest.makeSchedTest();
  return 0;
}
