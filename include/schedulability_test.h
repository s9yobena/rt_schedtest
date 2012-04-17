#ifndef SCHEDULABILITY_TEST_H
#define SCHEDULABILITY_TEST_H

/* *********************************************** */

#include <stdio.h>
#include <stdlib.h>


#include "litmus.h"

#define MAX_RT_TASK_NR 10

struct overhead_t {
  int cxs;
};

struct task_set_t {
 int  rt_task_id[MAX_RT_TASK_NR];
 struct rt_task rt_task_param[MAX_RT_TASK_NR];
 int nb_rts;
};

void task_set_init(struct task_set_t*);
int sched_test(struct task_set_t*);
int ovrhd_sched_test(struct task_set_t*, struct overhead_t*);
int get_nbr_cpus();



/* ********************************************* */


#endif
