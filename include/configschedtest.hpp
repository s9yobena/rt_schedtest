#ifndef CONFIG_SCHEDTEST_HPP
#define CONFIG_SCHEDTEST_HPP

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sys/wait.h>
#include <cstring>
#include <iostream>
#include "cmdlparser.h"
#include "rttypes.h" 		// for maxSTFname

using namespace std;

class ConfigSchedTest {
  
private:
  ConfigSchedTest();
  ConfigSchedTest(const ConfigSchedTest&);
  ConfigSchedTest& operator= (const ConfigSchedTest&);
  static ConfigSchedTest* configSchedTestInstance;
  
  char schedTestParamFile[maxSTFname];
  SchedTestName schedTestName;
  void initSchedTest(char*);

public:
  static ConfigSchedTest* getInstance();
  void callSchedTest();
  void configSchedTest();
  void setParameters(const CmdlParser&);
};

#endif
