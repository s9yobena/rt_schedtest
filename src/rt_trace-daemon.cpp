/*    rt_trace-daemon -- Overhead and scheduling tracing daemon
 *      
 *    Youcef A. Benabbas
 */
#include "litmusoverhead.hpp"
#include "litmusschedulingtrace.hpp"
#include "schedtestparam.hpp"

#include <cstdlib>
#include <cstdio>
#include <sstream>


static void setDevices() {

  LitmusDevice *litmusDevice;
  stringstream litmusDeviceName;

  litmusDevice = LitmusOverhead::getInstance();  
  litmusDeviceName.str("/dev/litmus/ft_trace0");  
  litmusDevice->initDev(litmusDeviceName.str().c_str());

  long nbrCPUs;
  nbrCPUs = sysconf(_SC_NPROCESSORS_ONLN);

  for (int i=0; i<nbrCPUs; i++) {
    litmusDeviceName.str("");
    litmusDevice = new LitmusSchedulingTrace();
    litmusDeviceName<<"/dev/litmus/sched_trace"<<i;
    litmusDevice->initDev(litmusDeviceName.str().c_str());
  }
}
void finishTesting(int sig)
{
  LitmusDevice::stopAllDevices();
  exit(0);
}

int main(int argc, char **argv) {

  SchedTestParam *schedTestParam;
  schedTestParam = SchedTestParam::getInstance();
  schedTestParam->setOutputName("schedtestfile.stf");
  schedTestParam->resetLocalParams();

  signal(SIGINT, finishTesting);
  signal(SIGUSR1, finishTesting);
  signal(SIGTERM, finishTesting);

  setDevices();
  LitmusDevice::startTracingAllDevices();
  return 0;
}
