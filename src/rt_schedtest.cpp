/*    rt_schedtest -- Online overhead aware schedulability testing
 *      
 *    Youcef A. Benabbas under the supervision of Dr. Björn B. Brandenburg,
 */
#include "cmdlparser.h"
#include "taskset.hpp"
#include "overhead.hpp"
#include "schedtest.hpp"
#include "litmusoverhead.hpp"
#include "litmusschedulingtrace.hpp"

#include <cstdlib>
#include <cstdio>
#include <sstream>

int getNbrCpus(){
  FILE * fp;
  char res[128];
  fp = popen("/bin/cat /proc/cpuinfo |grep -c '^processor'","r");
  fread(res, 1, sizeof(res)-1, fp);
  pclose(fp);
  return atoi(&res[0]);
}


static void setDevices(const CmdlParser& cmdlParser) {

  LitmusDevice *litmusDevice;
  stringstream litmusDeviceName;

  litmusDevice = LitmusOverhead::getInstance();  
  litmusDeviceName.str("/dev/litmus/ft_trace0");  
  litmusDevice->setParameters(cmdlParser);  
  litmusDevice->initDev(litmusDeviceName.str().c_str());

  int nbrCPUs = getNbrCpus();
  
  for (int i=0; i<nbrCPUs; i++) {
    litmusDeviceName.str("");
    litmusDevice = new LitmusSchedulingTrace();
    litmusDeviceName<<"/dev/litmus/sched_trace"<<i;
    litmusDevice->setParameters(cmdlParser);
    litmusDevice->initDev(litmusDeviceName.str().c_str());
  }
}

void finishTesting(int sig)
{
  LitmusDevice::stopAllDevices();
  exit(0);
}

int main(int argc, char **argv) {

  TaskSet *taskSet;
  Overhead *overhead;
  SchedTest schedTest;  
  CmdlParser cmdlParser(argc, argv);    

  overhead = Overhead::getInstance();
  taskSet = TaskSet::getInstance();

  signal(SIGINT, finishTesting);
  signal(SIGUSR1, finishTesting);
  signal(SIGTERM, finishTesting);

  schedTest.setTaskSet(taskSet);
  schedTest.setOverhead(overhead);
  overhead->setSchedTestObserver(&schedTest);

  taskSet->setParameters(cmdlParser);
  overhead->setParameters(cmdlParser);

  setDevices(cmdlParser);
  LitmusDevice::startTracingAllDevices();
  
  return 0;
}
