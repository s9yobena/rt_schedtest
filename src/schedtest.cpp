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

  sum_density = 0.0;
  max_density = 0.0;
  cur_density = 0.0;
  inflation = 0.0;
  
  inflation = (long double)(overhead->getCXS()+ overhead->getSCHED()
			    + overhead->getRELEASE() + overhead->getSEND_RESCHED()
			    + overhead->getRELEASE_LATENCY() + overhead->getCPMD()) ;

  for (i=0; i< this->taskSet->nb_rts; i++) {
    cur_density = 0.0;
    cur_density =
      (long double)( (long double) ( (long double)((this->taskSet->rt_task_param[i].exec_cost) / __NS_PER_MS)
				     +(long double)(inflation*this->getMsPerCycle()) 
				     ) 
		     /(long double)((this->taskSet->rt_task_param[i].period)/__NS_PER_MS));
    
    if (cur_density > max_density) {
      max_density = cur_density;
    }
    sum_density += cur_density;    
  }
  printf("\n");
  printf("******************************Testing schedulability********************************************\n");
  // printf("\n sum_density  ?< nbr_cpu - (nbr_cpu-1)*max_density: %Lf ?< %Lf",sum_density,nbr_cpu-(nbr_cpu-1)*max_density);
  // printf("\n sum_density: %Lf; \t nbr_cpu: %d: \t max_density: %Lf \n",sum_density, nbr_cpu, max_density);
  // printf("\n CXS(CYCLES): %llu; \t SCHED(CYCLES): %llu; \t SCHED2(CYCLES): %llu; \t RELEASE(CYCLES): %llu; \t SEND_RESCHED(CYCLES): %llu; \t RELEASE_LATENCY(CYCLES): %llu; \t CPMD(MS): %llu;\n",
  // 	 overhead->getCXS(), overhead->getSCHED(), overhead->getSCHED2(), overhead->getRELEASE(),
  // 	 overhead->getSEND_RESCHED(), overhead->getRELEASE_LATENCY(), overhead->getCPMD());

  std::cout<<"sum_density  ?< nbr_cpu - (nbr_cpu-1)*max_density:  "
	   <<sum_density
	   <<" ?< "
	   <<nbr_cpu-(nbr_cpu-1)*max_density
	   <<std::endl;

  std::cout<<"sum_density: "<<sum_density<<"\t"
	   <<"nbr_cpu: "<<nbr_cpu<<"\t"
	   <<"max_density: "<<max_density<<"\t"
	   <<"sum_overhead: "<<(long double)inflation*getMsPerCycle()<<"\t"
	   <<std::endl;

 cout<<endl;
  
  std::cout<<"CXS(Cycles): "<<overhead->getCXS() <<"\t"
	   <<"SCHED(Cycles): "<<overhead->getSCHED() <<"\t"
	   <<"SCHED2(Cycles): "<<overhead->getSCHED2() <<"\t"
    	   <<std::endl;
  std::cout <<"RELEASE(Cycles): "<<overhead->getRELEASE() <<"\t"
    	   <<"SEND_RESCHED(Cycles): "<<overhead->getSEND_RESCHED() <<"\t"
    	   <<"RELEASE_LATENCY(Cycles): "<<overhead->getRELEASE_LATENCY() <<"\t"
	   <<std::endl;  

  cout<<endl;

  std::cout<<"CPU CLOCK MHz: "<<__CPU_CLOCK_MHz<<"\t"
	   <<"MS PER CYCLE: "<<getMsPerCycle()<<"\t"
	   <<std::endl;

  cout<<endl;

  std::cout<<"CXS(MS): "<<overhead->getCXS()*(long double)getMsPerCycle()<<"\t"
	   <<"SCHED(MS): "<<overhead->getSCHED()*(long double)getMsPerCycle()<<"\t"
	   <<"SCHED2(MS): "<<overhead->getSCHED2()*(long double)getMsPerCycle()<<"\t"
	   <<std::endl;
  std::cout<<"RELEASE(MS): "<<overhead->getRELEASE()*(long double)getMsPerCycle()<<"\t"
    	   <<"SEND_RESCHED(MS): "<<overhead->getSEND_RESCHED()*(long double)getMsPerCycle()<<"\t"
    	   <<"RELEASE_LATENCY(MS): "<<overhead->getRELEASE_LATENCY()*(long double)getMsPerCycle()<<"\t"
	   <<std::endl;

  cout<<endl;

  
  if (sum_density <= (long double)nbr_cpu-((long double)nbr_cpu-1)*max_density) {
    printf("\n*******************Schedulability test result:***** TASK SET STILL SCHEDULABLE :-) ****************** \n");
    return 1;
  } else {
    printf("\n*******************Schedulability test result:***** TASK SET NO LONGER SCHEDULABLE :-( ************** \n");
    return 0;
  }  
  printf("\n");
  printf("\n");

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

long double SchedTest::getMsPerCycle() {

  long double sPerCycle;
  sPerCycle = 1.0/(__Hz_PER_MHz * __CPU_CLOCK_MHz);
  msPerCycle = sPerCycle * __MS_PER_S;
  return this->msPerCycle;
}

   
