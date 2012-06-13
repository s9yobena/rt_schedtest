#include "overhead.hpp"


Overhead* Overhead::overheadInstance = 0;
SchedTest* Overhead::schedTest = 0;


Overhead::Overhead() {
  this->CXS = 0;
  this->SCHED = 0;
  this->SCHED2 = 0;
  this->RELEASE = 0;
  this->RELEASE_LATENCY = 0;
  this->SEND_RESCHED = 0;
  this->CPMD = 0;

}

Overhead* Overhead::getInstance() {
  if (!overheadInstance) {
    overheadInstance = new Overhead();
  }
  return overheadInstance;
}

overhead_t Overhead::getCXS() {
  return this->CXS;
}

overhead_t Overhead::getSCHED() {
  return this->SCHED;
}

overhead_t Overhead::getSCHED2() {
  return this->SCHED2;
}

overhead_t Overhead::getRELEASE() {
  return this->RELEASE;
}

overhead_t Overhead::getRELEASE_LATENCY() {
  return this->RELEASE_LATENCY;
}

overhead_t Overhead::getSEND_RESCHED() {
  return this->SEND_RESCHED;
}

overhead_t Overhead::getCPMD() {
  return this->CPMD;
}

void Overhead::updateAllOverheads(SchedTestParam *schedTestParam) {
  CXS = schedTestParam->getCXS();
  SCHED = schedTestParam->getSCHED();
  SCHED2 = schedTestParam->getSCHED2();
  RELEASE = schedTestParam->getRELEASE();
  SEND_RESCHED = schedTestParam->getSEND_RESCHED();
  RELEASE_LATENCY = schedTestParam->getRELEASE_LATENCY();
}

void Overhead::updateCXS(overhead_t  cxs){
  this->CXS = cxs;
}

void Overhead::updateSCHED(overhead_t  sched){
  this->SCHED = sched;
}

void Overhead::updateSCHED2(overhead_t  sched2){
  this->SCHED2 = sched2;
}

void Overhead::updateRELEASE(overhead_t  release){
  this->RELEASE = release;
}

void Overhead::updateRELEASE_LATENCY(overhead_t  release_latency){
  this->RELEASE_LATENCY = release_latency;
}

void Overhead::updateSEND_RESCHED(overhead_t  send_resched){
  this->SEND_RESCHED = send_resched;
}

void Overhead::updateCPMD(overhead_t  cpmd){
  this->CPMD = cpmd;
}

void Overhead::setParameters(const CmdlParser& cmdlParser) {
  this->CPMD = cmdlParser.cpmd;
}


void Overhead::setSchedTestObserver(SchedTest *schedTest) {
  this->schedTest = schedTest;
}

void Overhead::updateSchedTest() {
  this->schedTest->makeSchedTest();
}
