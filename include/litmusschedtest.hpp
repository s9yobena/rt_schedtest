#ifndef LITMUS_SCHEDTEST_HPP
#define LITMUS_SCHEDTEST_HPP

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sys/wait.h>
#include <iostream>

using namespace std;

class LitmusSchedTest {
  
private:
  LitmusSchedTest();
  LitmusSchedTest(const LitmusSchedTest&);
  LitmusSchedTest& operator= (const LitmusSchedTest&);
  static LitmusSchedTest* litmusSchedTestInstance;

  void initSchedTest(char*);

public:
  static LitmusSchedTest* getInstance();
  void callSchedTest(char*);
};

#endif
