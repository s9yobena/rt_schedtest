#include "litmusdaemonlistner.hpp"

LitmusDaemonListner* LitmusDaemonListner::litmusDaemonListnerInstance = 0;

LitmusDaemonListner::LitmusDaemonListner() {
  setLitmusDaemonName("./rt_trace-daemon");
  initLitmusDaemonListner();
}

LitmusDaemonListner* LitmusDaemonListner::getInstance() {
    if (!litmusDaemonListnerInstance) {
    litmusDaemonListnerInstance = new LitmusDaemonListner();
  }
  return litmusDaemonListnerInstance;
}

void LitmusDaemonListner::receiveData() {

  int rd;
  size_t size, count;
  LitmusTracingPacket *ts, *end;
  
  
  while ( (rd = fread(daemonBuffer, 1, sizeof(struct LitmusTracingPacket), daemonFD))> 0) {

    size = rd;
    ts = (struct LitmusTracingPacket*) daemonBuffer;
    count = size / sizeof(struct  LitmusTracingPacket);
    end = ts + count;
    
    for (; ts != end; ts++){
      notifyDataReceived(ts);
    }
  }
}

void LitmusDaemonListner::initLitmusDaemonListner() {

  daemonFD = popen(litmusDaemonName,"r");
}


void LitmusDaemonListner::notifyDataReceived(LitmusTracingPacket *tp) {

  // overheadObserver->updateCXS(tp->getCXS());
  // overheadObserver->updateSCHED(tp->getSCHED());
  // overheadObserver->updateSCHED2(tp->getSCHED2());
  // overheadObserver->updateRELEASE(tp->getRELEASE());
  // overheadObserver->updateRELEASE_LATENCY(tp->getRELEASE_LATENCY());
  // overheadObserver->updateSEND_RESCHED(tp->getSEND_RESCHED());
  
  overheadObserver->updateCXS(tp->CXS);
  overheadObserver->updateSCHED(tp->SCHED);
  overheadObserver->updateSCHED2(tp->SCHED2);
  overheadObserver->updateRELEASE(tp->RELEASE);
  overheadObserver->updateRELEASE_LATENCY(tp->RELEASE_LATENCY);
  overheadObserver->updateSEND_RESCHED(tp->SEND_RESCHED);

  overheadObserver->updateSchedTest();
}

void LitmusDaemonListner::setLitmusDaemonName(const char* litmusDaemonName) {
  this->litmusDaemonName = litmusDaemonName;
}

void LitmusDaemonListner::setOverheadObserver(Overhead *overheadObserver) {
  
  this->overheadObserver = overheadObserver;
}
