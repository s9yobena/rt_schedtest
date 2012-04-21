#include "litmustimestamp.hpp"

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

