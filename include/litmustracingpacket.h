#ifndef LITMUS_TRACING_PACKET_H
#define LITMUS_TRACING_PACKET_H

#include <inttypes.h>
#include "schedtrace.h"

typedef unsigned long long overhead_t;


struct LitmusCXSData {
  overhead_t overhead;
  uint8_t __unused[2];
};

struct LitmusSCHEDData {
  overhead_t overhead;
  uint8_t __unused[2];
};

struct LitmusSCHED2Data {
  overhead_t overhead;
  uint8_t __unused[2];
};

struct LitmusRELEASEData {
  overhead_t overhead;
  uint8_t __unused[2];
};

struct LitmusRELEASE_LATENCYData {
  overhead_t overhead;
  uint8_t __unused[2];
};

struct LitmusSEND_RESCHEDData {
  overhead_t overhead;
  uint8_t __unused[2];
};

struct LitmusExecutionCostData {
  task_id_t task_id;
  exec_time_t exec_time;
};

struct LitmusInterArrivalTimeData {
  task_id_t task_id;
  exec_time_t exec_time;  
};

struct LitmusTracingPacket {
  uint8_t id;
  union {
    struct LitmusCXSData litmusCXSData;
    struct LitmusSCHEDData litmusSCHEDData;
    struct LitmusSCHED2Data litmusSCHED2Data;
    struct LitmusRELEASEData litmusRELEASEData;
    struct LitmusRELEASE_LATENCYData litmusRELEASE_LATENCYData;
    struct LitmusSEND_RESCHEDData litmusSEND_RESCHEDData;
    struct LitmusExecutionCostData litmusExecutionCostData;
    struct LitmusInterArrivalTimeData litmusInterArrivalTimeData ;
  } data;
};

typedef enum {

  LitmusCXSDataID,
  LitmusSCHEDDataID,
  LitmusSCHED2DataID,
  LitmusRELEASEDataID,
  LitmusRELEASE_LATENCYDataID,
  LitmusSEND_RESCHEDDataID,
  LitmusExecutionCostDataID,
  LitmusInterArrivalTimeDataID

} LitmusTracingPacketID;



#endif
