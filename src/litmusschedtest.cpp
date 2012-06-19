#include "litmusschedtest.hpp"

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
  } 
}

void LitmusSchedTest::initSchedTest(char *schedTestParamFile) {
  
  char schedTestNameOption[25];
  strcpy(schedTestNameOption, "--");
  strcat(schedTestNameOption, schedTestName);
  execl( "./rt_schedtest","rt_schedtest", 
	 schedTestNameOption,
	 "--stf", schedTestParamFile,
	 (char *) NULL );
  perror( "execl()" );
  exit(EXIT_FAILURE);
}

void LitmusSchedTest::setDensityTest() {
  strcpy(schedTestName, "density-test");
}
