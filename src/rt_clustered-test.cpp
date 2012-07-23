/*    rt_partitionned-test
 *      
 *    Youcef A. Benabbas.
 */
#include "cmdlparser.h"
#include "taskset.hpp"
#include "overhead.hpp"
#include "cachetop.hpp"
#include "clusteredtest.hpp"
#include "schedtestparam.hpp"


int main(int argc, char **argv) {

  TaskSet *taskSet;
  Overhead *overhead;
  CacheTop *cacheTop;
  ClusteredTest clusteredTest;  
  SchedTestParam *schedTestParam;
  CmdlParser cmdlParser(argc, argv);    

  overhead = Overhead::getInstance();
  taskSet = new TaskSet();
  cacheTop = new CacheTop();
  schedTestParam = SchedTestParam::getInstance();

  clusteredTest.setTaskSet(taskSet);
  clusteredTest.setOverhead(overhead);
  clusteredTest.setCacheTop(cacheTop);

  schedTestParam->initSchedTestParam();
  schedTestParam->setParameters(cmdlParser);
  schedTestParam->getSchedTestParam();

  overhead->updateAllOverheads(schedTestParam);
  taskSet->updateAllTasks(schedTestParam);
  cacheTop->updateCacheTop(schedTestParam);

  taskSet->setParameters(cmdlParser);
  overhead->setParameters(cmdlParser);

  clusteredTest.setMHzCpuClock(schedTestParam->getMHzCpuClock());
  clusteredTest.makeSchedTest();
  return 0;
}
