#ifndef LITMUS_DAEMON_HPP
#define LITMUS_DAEMON_HPP

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <sys/wait.h>
#include <iostream>
#include "litmustracingpacket.h"

using namespace std;

class LitmusDaemon {
  
private:
  LitmusDaemon();
  LitmusDaemon(const LitmusDaemon&);
  LitmusDaemon& operator= (const LitmusDaemon&);
  static LitmusDaemon* litmusDaemonInstance;

  int litmusDaemonPipe[2];
  
  void initLitmusDaemonPipe();
  void initSchedTest();
  void initParent();

public:
  static LitmusDaemon* getInstance();
  void forkSchedTest();
};

#endif
