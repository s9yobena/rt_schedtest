#include "timestamp.h"
#include "litmustimestamp.hpp"


#ifndef SINGLE_LITMUS_TIMESTAMP_HPP
#define SINGLE_LITMUS_TIMESTAMP_HPP


class LitmusTimestamp;

class SingleLitmusTimestamp : public LitmusTimestamp {

public:
  SingleLitmusTimestamp(cmd_t);
  void check(struct timestamp*);

};

#endif
