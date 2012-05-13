#ifndef PACKET_2_STDOUT_HPP
#define PACKET_2_STDOUT_HPP

#include <unistd.h>
#include "litmustracingpacket.h"


class Packet2Stdout {

private:

  Packet2Stdout();
  Packet2Stdout(const Packet2Stdout&);
  Packet2Stdout& operator= (const Packet2Stdout&);
  static Packet2Stdout *packet2StdoutInstance;

  struct LitmusTracingPacket packetToSend;

public:
  static Packet2Stdout* getInstance();
  void send();

  void setCXS(overhead_t);
  void setSCHED(overhead_t);
  void setSCHED2(overhead_t);
  void setRELEASE(overhead_t);
  void setRELEASE_LATENCY(overhead_t);
  void setSEND_RESCHED(overhead_t);
  
  void setTaskExecutionCost(exec_time_t, task_id_t);  
  void setTaskInterArrivalTime(exec_time_t, task_id_t);

};

#endif

