#include "schedtest.hpp"

SchedTest::SchedTest() {
  this->schedTestName = DENSITY_TEST;
  this->nbr_cpu =  get_nbr_cpus();
}

SchedTest::~SchedTest() {
}

void SchedTest::setTaskSet(TaskSet *taskSet) {
  this->taskSet = taskSet;
}

void SchedTest::setOverhead(Overhead *overhead) {
  this->overhead = overhead;
}


int SchedTest::makeSchedTest() {
  
  switch (this->schedTestName) {
  case DENSITY_TEST:
    return(this->densityTest());
  default:
    break;
  }
    
}

int SchedTest::densityTest() {
  
  int i;
  long double sum_density;
  long double max_density;
  long double cur_density;
  long double inflation;
  sum_density = 0;
  max_density = 0;
  cur_density = 0;
  inflation = (long double)(overhead->getCXS()+ overhead->getSCHED() + overhead->getRELEASE() + overhead->getSEND_RESCHED() + overhead->getRELEASE_LATENCY() + overhead->getCPMD()) ;
  for (i=0; i< this->taskSet->nb_rts; i++){
    cur_density =(long double)(this->taskSet->rt_task_param[i].exec_cost+(inflation*__US_PER_MS)) /(long double)this->taskSet->rt_task_param[i].period;
    if (cur_density > max_density) {
      max_density = cur_density;
    }
    sum_density += cur_density;    
  }
  
  printf("\n sum_density  ?< nbr_cpu - (nbr_cpu-1)*max_density: %Lf ?< %Lf",sum_density,nbr_cpu-(nbr_cpu-1)*max_density);
  printf("\n sum_density: %Lf; \t nbr_cpu: %d: \t max_density: %Lf \n",sum_density, nbr_cpu, max_density);
  printf("\n CXS: %llu; \t SCHED: %llu; \t SCHED2: %llu; \t RELASE: %llu; \t SEND_RESCHED: %llu; \t RELEASE_LATENCY: %llu; \t CPMD: %llu;\n",
	 overhead->getCXS(), overhead->getSCHED(), overhead->getSCHED2(), 
	 overhead->getRELEASE(), overhead->getSEND_RESCHED(), overhead->getRELEASE_LATENCY(), overhead->getCPMD());
  
  if (sum_density <= (long double)nbr_cpu-((long double)nbr_cpu-1)*max_density) {
    printf("\n ********** TASK SET STILL SCHEDULABLE :-) ****************** \n");
    return 1;
  } else {
    printf("\n ********** TASK SET NO LONGER SCHEDULABLE :-( ****************** \n");
    return 0;
  }  
}


int SchedTest::get_nbr_cpus(){
        FILE * fp;
        char res[128];
        fp = popen("/bin/cat /proc/cpuinfo |grep -c '^processor'","r");
        fread(res, 1, sizeof(res)-1, fp);
        pclose(fp);
	/* printf("nbr of cpus: %c \n",res[0]); */
        return atoi(&res[0]);
}
