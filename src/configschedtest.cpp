#include "configschedtest.hpp"

ConfigSchedTest::ConfigSchedTest() {
}

ConfigSchedTest* ConfigSchedTest::configSchedTestInstance = 0;

ConfigSchedTest* ConfigSchedTest::getInstance() {
  
  if (!configSchedTestInstance) {
    configSchedTestInstance = new ConfigSchedTest();
  }
  return configSchedTestInstance;
}


void ConfigSchedTest::callSchedTest() {
  
  pid_t pid;
  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  // sched test
  if (pid == 0) {
    configSchedTest();
  } 
}

void ConfigSchedTest::configSchedTest() {

  switch (schedTestName) {
  case DENSITY_TEST:
    execl( "./rt_density-test","rt_density-test","--stf", schedTestParamFile,(char *) NULL );
    perror( "execl()" );
    exit(EXIT_FAILURE);

  case P_EDF_TEST:
    execl( "./rt_partitionned-test","rt_partitionned-test","--stf", schedTestParamFile,(char *) NULL );
    perror( "execl()" );
    exit(EXIT_FAILURE);

  case C_EDF_TEST:
    execl( "./rt_clustered-test","rt_clustered-test","--stf", schedTestParamFile,(char *) NULL );
    perror( "execl()" );
    exit(EXIT_FAILURE);

  }
}

void ConfigSchedTest::setParameters(const CmdlParser& cmdlParser) {
  schedTestName = cmdlParser.schedTestName;
  strcpy(schedTestParamFile, cmdlParser.stfName);
}
