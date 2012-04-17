#include "schedulability_test.h"


void task_set_init(struct task_set_t* task_set) {
  task_set->nb_rts = 0;
}

/* ************************************************************** */

/* Test if the task set is schedulabale using the density test.
 * GSN-EDF "global EDF", is used.
 * Currently overheads are not taken care of.
 * Implicit deadlines are assumed. i.e Deadline = Period.
 * 
 * TODO: retrieve number of processors
 * TODO: Fix time measurement
 */
int sched_test(struct task_set_t* task_set) {

  int i;
  long double sum_density;
  long double max_density;
  long double cur_density;
  int nbr_cpu;
  nbr_cpu = get_nbr_cpus();
  sum_density = 0;
  max_density = 0;
  cur_density = 0;
  for (i=0; i<task_set->nb_rts; i++){
    cur_density =(long double)task_set->rt_task_param[i].exec_cost /(long double)task_set->rt_task_param[i].period;
    if (cur_density > max_density) {
      max_density = cur_density;
    }
    sum_density += cur_density;
    
  }
  
  printf("sum_density  ?<> nbr_cpu - (nbr_cpu-1)*max_density: %Lf ?<> %Lf",sum_density,nbr_cpu-(nbr_cpu-1)*max_density);
  
  if (sum_density <= (long double)nbr_cpu-((long double)nbr_cpu-1)*max_density) {
    return 1;
  } else {
    return 0;
  }  

}

int ovrhd_sched_test(struct task_set_t* task_set, struct overhead_t* overhead) {

  int i;
  long double sum_density;
  long double max_density;
  long double cur_density;
  int nbr_cpu;
  nbr_cpu = get_nbr_cpus();
  sum_density = 0;
  max_density = 0;
  cur_density = 0;
  for (i=0; i<task_set->nb_rts; i++){
    cur_density =(long double)(task_set->rt_task_param[i].exec_cost+(overhead->cxs*__NS_PER_MS)) /(long double)task_set->rt_task_param[i].period;
    if (cur_density > max_density) {
      max_density = cur_density;
    }
    sum_density += cur_density;
    
  }
  
  printf("sum_density  ?<> nbr_cpu - (nbr_cpu-1)*max_density: %Lf ?<> %Lf",sum_density,nbr_cpu-(nbr_cpu-1)*max_density);
  
  if (sum_density <= (long double)nbr_cpu-((long double)nbr_cpu-1)*max_density) {
    return 1;
  } else {
    return 0;
  }  

}


int get_nbr_cpus(){
        FILE * fp;
        char res[128];
        fp = popen("/bin/cat /proc/cpuinfo |grep -c '^processor'","r");
        fread(res, 1, sizeof(res)-1, fp);
        pclose(fp);
	/* printf("nbr of cpus: %c \n",res[0]); */
        return atoi(&res[0]);
}



