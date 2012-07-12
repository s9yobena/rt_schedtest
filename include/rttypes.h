#ifndef RT_TYPES
#define RT_TYPES
#include <stdint.h>
#include "litmus/rt_param.h"	/* for lt_t */
#include "litmus.h"		/* for pid_t */

enum SchedTestName {
  DENSITY_TEST,
  P_EDF_TEST,
  BAKERS_TEST
};

typedef unsigned long long overhead_t;

#endif
