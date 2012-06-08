#include "litmusdaemon.hpp"

LitmusDaemon::LitmusDaemon() {
}

LitmusDaemon* LitmusDaemon::litmusDaemonInstance = 0;

LitmusDaemon* LitmusDaemon::getInstance() {
  
  if (!litmusDaemonInstance) {
    litmusDaemonInstance = new LitmusDaemon();
  }
  return litmusDaemonInstance;
}


void LitmusDaemon::forkSchedTest() {
  
  initLitmusDaemonPipe();

  pid_t pid;
  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  // sched test
  if (pid == 0) {
    initSchedTest();
  } 
  // parent
  else {
    initParent();
  }
}

void LitmusDaemon::initLitmusDaemonPipe() {

  if (pipe(litmusDaemonPipe) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }
}

void LitmusDaemon::initSchedTest() {
  
  close(litmusDaemonPipe[1]);		// close write end of pipe
  dup2(litmusDaemonPipe[0], STDIN_FILENO);
  execv( "./rt_schedtest", NULL );
  perror( "execv()" );
  exit(255);
}

void LitmusDaemon::initParent() {

  close(litmusDaemonPipe[0]);		// close write end of pipe
  dup2(litmusDaemonPipe[1], STDOUT_FILENO);
}
