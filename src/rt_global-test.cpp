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

  double non_sched_t;

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

  if (!globalTest.makeSchedTest()) {

	  non_sched_t = wctime();
	  FILE *non_sched_file;
	  non_sched_file = fopen("sched_log_file","a");
	  char buf[100];
	  sprintf(buf,"%f\n", non_sched_t);
	  fputs(buf,non_sched_file);
	  fclose(non_sched_file);
	  
  } else  {

  	  FILE *non_sched_file;
  	  non_sched_file = fopen("sched_log_file","a");
  	  char buf[100];
  	  sprintf(buf,"TaskSetSchedulable\n");
  	  fputs(buf,non_sched_file);
  	  fclose(non_sched_file);
  }

  return 0;
}
