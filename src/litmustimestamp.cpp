#include "litmustimestamp.hpp"

unsigned int LitmusTimestamp::lastSeqNo = 0;

LitmusTimestamp::LitmusTimestamp() {
}

LitmusTimestamp::LitmusTimestamp(cmd_t id) {
  this->startID = id;
}

void LitmusTimestamp::setLitmusTimestampObserver(TimestampProcessor* timestampProcessor) {
  this->timestampProcessor = timestampProcessor;
}

void LitmusTimestamp::updateLitmusTimestampObservers(overhead_t overhead, cmd_t id) {
  this->timestampProcessor->notifyNewOverhead(overhead, id);
}

void LitmusTimestamp::setLastSeqNo(unsigned int _lastSeqNo) {
  lastSeqNo = _lastSeqNo;
}

unsigned int LitmusTimestamp::getLastSeqNo() {
  return lastSeqNo;
}
