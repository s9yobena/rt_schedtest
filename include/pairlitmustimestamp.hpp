#include <iostream>
#include <vector>
#include "timestamp.h"
#include "litmustimestamp.hpp"

using namespace std;

#ifndef PAIR_LITMUS_TIMESTAMP_HPP
#define PAIR_LITMUS_TIMESTAMP_HPP

class PairLitmusTimestamp : public LitmusTimestamp {

private:

  LitmusTimestampState state;
  struct timestamp currentTimestamp;
public:

  PairLitmusTimestamp(cmd_t);
  void check(struct timestamp*);
};

#endif
