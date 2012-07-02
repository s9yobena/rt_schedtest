#include <iostream>
#include <vector>
#include "timestamp.h"
#include "litmustimestamp.hpp"

using namespace std;

#ifndef TICK_LITMUS_TIMESTAMP_HPP
#define TICK_LITMUS_TIMESTAMP_HPP

class TickLitmusTimestamp : public LitmusTimestamp {

private:

  LitmusTimestampState state;
  struct timestamp currentTimestamp;
public:

  TickLitmusTimestamp(cmd_t);
  void check(struct timestamp*);
};

#endif
