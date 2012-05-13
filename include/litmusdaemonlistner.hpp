#ifndef LITMUS_DAEMON_LISTNER_HPP
#define LITMUS_DAEMON_LISTNER_HPP

#include<iostream>
#include <stdio.h>
#include "litmustracingpacket.h"
#include "overhead.hpp"

#define DAEMON_BUF_SIZE 4012

using namespace std;

class LitmusDaemonListner {
  
private:
  
  static LitmusDaemonListner* litmusDaemonListnerInstance;
  const char* litmusDaemonName;
  Overhead *overheadObserver;
  char daemonBuffer[DAEMON_BUF_SIZE];
  FILE *daemonFD;

  LitmusDaemonListner();  
  void initLitmusDaemonListner();
  void notifyDataReceived(LitmusTracingPacket *tf);

public:
  
  static LitmusDaemonListner* getInstance();
  void receiveData();
  void setLitmusDaemonName(const char* litmusDaemonName);
  void setOverheadObserver(Overhead *overheadObserver);
};


#endif
