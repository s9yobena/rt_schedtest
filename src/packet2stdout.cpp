#include "packet2stdout.hpp"

Packet2Stdout::Packet2Stdout() {
}

Packet2Stdout* Packet2Stdout::packet2StdoutInstance = 0;

Packet2Stdout* Packet2Stdout::getInstance() {
  
  if (!packet2StdoutInstance) {
    packet2StdoutInstance = new Packet2Stdout();
  }
  return packet2StdoutInstance;
}

void Packet2Stdout::send() {

  write(1, &packetToSend, sizeof(struct LitmusTracingPacket));
}

void Packet2Stdout::setCXS(overhead_t cxs) {
  packetToSend.id = LitmusCXSDataID;
  packetToSend.data.litmusCXSData.overhead = cxs;
}

void Packet2Stdout::setSCHED(overhead_t sched) {
  packetToSend.id = LitmusSCHEDDataID;
  packetToSend.data.litmusSCHEDData.overhead = sched;
}

void Packet2Stdout::setSCHED2(overhead_t sched) {
  packetToSend.id = LitmusSCHED2DataID;
  packetToSend.data.litmusSCHED2Data.overhead = sched;
}

void Packet2Stdout::setRELEASE(overhead_t release) {
  packetToSend.id = LitmusRELEASEDataID;
  packetToSend.data.litmusRELEASEData.overhead = release;
}

void Packet2Stdout::setRELEASE_LATENCY(overhead_t release_latency) {
  packetToSend.id = LitmusRELEASE_LATENCYDataID;
  packetToSend.data.litmusRELEASE_LATENCYData.overhead = release_latency;
}

void Packet2Stdout::setSEND_RESCHED(overhead_t send_resched) {
  packetToSend.id = LitmusSEND_RESCHEDDataID;
  packetToSend.data.litmusSEND_RESCHEDData.overhead = send_resched;
}

void Packet2Stdout::setTaskExecutionCost(exec_time_t exec_time, task_id_t task_id) {
  packetToSend.id = LitmusExecutionCostDataID;
  packetToSend.data.litmusExecutionCostData.exec_time = exec_time;
  packetToSend.data.litmusExecutionCostData.task_id = task_id;
}

void Packet2Stdout::setTaskInterArrivalTime(exec_time_t exec_time, task_id_t task_id) {
  packetToSend.id = LitmusInterArrivalTimeDataID;
  packetToSend.data.litmusInterArrivalTimeData.exec_time = exec_time;
  packetToSend.data.litmusInterArrivalTimeData.task_id = task_id;
}




