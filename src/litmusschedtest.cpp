#include "litmusschedtest.hpp"
#include "globaltest.hpp"
#include "cpuclock.hpp"
#include "overhead.hpp"

#include <cstdio>
#include <cstring>

struct plugin_name {
  const char *name;
  const char *schedtestname;
};

static struct plugin_name plugin_table [] = {
  
  {"GSN-EDF","g-edf-test"},
  {"PSN-EDF","p-edf-test"},
  {"C-EDF","c-edf-test"}
};


LitmusSchedTest::LitmusSchedTest() {
}

LitmusSchedTest* LitmusSchedTest::litmusSchedTestInstance = 0;

LitmusSchedTest* LitmusSchedTest::getInstance() {
  
  if (!litmusSchedTestInstance) {
    litmusSchedTestInstance = new LitmusSchedTest();
  }
  return litmusSchedTestInstance;
}


void LitmusSchedTest::callSchedTest(char *schedTestParamFile) {

  pid_t pid;
  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  // sched test
  if (pid == 0) {
    initSchedTest(schedTestParamFile);    
    execl( "./rt_schedtest","rt_schedtest", 
  	   schedTestNameOption,
  	   "--stf", schedTestParamFile,
  	   (char *) NULL );
    perror( "execl()" );
    exit(EXIT_FAILURE);
  } 
}



void LitmusSchedTest::initSchedTest(char *schedTestParamFile) {
  
  FILE *active_plugin_f;

  char active_plugin[25];
  
  active_plugin_f = fopen("/proc/litmus/active_plugin","r");
  fgets(active_plugin,sizeof active_plugin,active_plugin_f);
  
  for (unsigned i=0; i<(sizeof(plugin_table)/sizeof(plugin_table[0])); i++) {
    if (!strncmp(active_plugin, plugin_table[i].name, strlen(plugin_table[i].name))){
	strcpy(schedTestName, plugin_table[i].schedtestname);
      }
  }
#ifdef PRINT_SCHED_OUTPUT
  printf("callign rt_schedtest with the following schedulability test: %s \n ",schedTestName);
#endif

  strcpy(schedTestNameOption, "--");
  strcat(schedTestNameOption, schedTestName);
}

void LitmusSchedTest::doSchedTest() {

  GlobalTest globalTest;
  double non_sched_t;

  globalTest.setTaskSet(taskSet);
  globalTest.setOverhead(Overhead::getInstance());

  globalTest.setMHzCpuClock(CpuClock::getMHzCpuClock());
  globalTest.setNbrCpus(sysconf(_SC_NPROCESSORS_ONLN));

  if (!globalTest.makeSchedTest()) {

	  non_sched_t = wctime();
	  FILE *non_sched_file;
	  non_sched_file = fopen("sched_log_file","a");
	  char buf[100];
	  sprintf(buf,"TaskSet unschedulable at %f\n", non_sched_t);
	  fputs(buf,non_sched_file);
	  fclose(non_sched_file);
	  // printf("TaskSet unschedulable at %f\n", non_sched_t);
	  
  } // else  {

  // 	  FILE *non_sched_file;
  // 	  non_sched_file = fopen("sched_log_file","a");
  // 	  char buf[100];
  // 	  sprintf(buf,"TaskSet schedulable at %f\n", wctime());
  // 	  fputs(buf,non_sched_file);
  // 	  fclose(non_sched_file);
  // }

}

void LitmusSchedTest::setTaskSet(TaskSet *_taskSet) {

  taskSet = _taskSet;
}


