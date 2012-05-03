#include <iostream>
#include <vector>
#include "timestamp.h"
#include "litmustimestamp.hpp"

using namespace std;


#ifndef SEND_RESCHED_LITMUS_TIMESTAMP_HPP
#define SEND_RESCHED_LITMUS_TIMESTAMP_HPP

class SendReschedLitmusTimestamp : public LitmusTimestamp {

private:

  LitmusTimestampState state;
  struct timestamp currentTimestamp;
public:

  SendReschedLitmusTimestamp(cmd_t);
  void check(struct timestamp*);
};

#endif
