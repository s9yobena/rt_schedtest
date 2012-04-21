#include <iostream>
#include <vector>
#include "timestamp.h"
#include "litmustimestamp.hpp"

using namespace std;


#ifndef PAIR_LITMUS_TIMESTAMP_HPP
#define PAIR_LITMUS_TIMESTAMP_HPP

// TODO: remove this
#define MAX_NBR_CPU 32

struct CPUTimestamp {
  struct timestamp ts;
  int state;
  CPUTimestamp() {
    state = 0;
  }
};

class PairLitmusTimestamp : public LitmusTimestamp {

private:
  vector<CPUTimestamp> currentTimestamp;
public:
  PairLitmusTimestamp(cmd_t);
  void check(struct timestamp*);
};

#endif
