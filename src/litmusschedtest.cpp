#include "litmusschedtest.hpp"
#include <cstdio>
#include <cstring>

struct plugin_name {
  const char *name;
  const char *schedtestname;
};

static struct plugin_name plugin_table [] = {
  
  {"GSN-EDF","density-test"},
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
  
  for (int i=0; i<(sizeof(plugin_table)/sizeof(plugin_table[0])); i++) {
    if (!strncmp(active_plugin, plugin_table[i].name, strlen(plugin_table[i].name))){
	strcpy(schedTestName, plugin_table[i].schedtestname);
      }
  }
  printf("callign rt_schedtest with the following schedulability test: %s \n ",schedTestName);

  strcpy(schedTestNameOption, "--");
  strcat(schedTestNameOption, schedTestName);
}

