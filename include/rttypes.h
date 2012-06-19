#ifndef RT_TYPES
#define RT_TYPES
#include <stdint.h>
#include "litmus/rt_param.h"	/* for lt_t */

enum SchedTestName {
  DENSITY_TEST,
  BAKERS_TEST
};

typedef unsigned long long overhead_t;

#endif
