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
			    + (overhead->getRELEASE_LATENCY())
			    + overhead->getCPMD()) ;

  inflation = (long double)(inflation*this->getNsPerCycle()) + overhead->getRELEASE_LATENCY();

  for (i=0; i< this->taskSet->nb_rts; i++) {
    cur_density = 0.0;
    cur_density =
      (long double)( (long double) ( (long double)((this->taskSet->rt_task_param[i].exec_cost))
				     +(long double)(inflation) 
				     ) 
		     /(long double)((this->taskSet->rt_task_param[i].period)));
    
    if (cur_density > max_density) {
      max_density = cur_density;
    }
    sum_density += cur_density;    
  }
  printf("\n");
  printf("******************************Testing schedulability********************************************\n");

  std::cout<<"sum_density  ?< nbr_cpu - (nbr_cpu-1)*max_density:  "
	   <<sum_density
	   <<" ?< "
	   <<nbr_cpu-(nbr_cpu-1)*max_density
	   <<std::endl;

  std::cout<<"sum_density(NS): "<<sum_density<<"\t"
	   <<"nbr_cpu: "<<nbr_cpu<<"\t"
	   <<"max_density(NS): "<<max_density<<"\t"
	   <<"sum_overhead(NS): "<<(long double)inflation<<"\t"
	   <<std::endl;

 cout<<endl;
  
  std::cout<<"CXS(Cycles): "<<overhead->getCXS() <<"\t"
	   <<"SCHED(Cycles): "<<overhead->getSCHED() <<"\t"
	   <<"SCHED2(Cycles): "<<overhead->getSCHED2() <<"\t"
    	   <<std::endl;
  std::cout <<"RELEASE(Cycles): "<<overhead->getRELEASE() <<"\t"
    	   <<"SEND_RESCHED(Cycles): "<<overhead->getSEND_RESCHED() <<"\t"
    	   <<"RELEASE_LATENCY(NS): "<<overhead->getRELEASE_LATENCY() <<"\t"
	   <<std::endl;  

  cout<<endl;

  std::cout<<"CPU CLOCK MHz: "<<__CPU_CLOCK_MHz<<"\t"
	   <<"NS PER CYCLE: "<<getNsPerCycle()<<"\t"
	   <<std::endl;

  cout<<endl;

  std::cout<<"CXS(NS): "<<overhead->getCXS()*(long double)getNsPerCycle()<<"\t"
	   <<"SCHED(NS): "<<overhead->getSCHED()*(long double)getNsPerCycle()<<"\t"
	   <<"SCHED2(NS): "<<overhead->getSCHED2()*(long double)getNsPerCycle()<<"\t"
	   <<std::endl;
  std::cout<<"RELEASE(NS): "<<overhead->getRELEASE()*(long double)getNsPerCycle()<<"\t"
    	   <<"SEND_RESCHED(NS): "<<overhead->getSEND_RESCHED()*(long double)getNsPerCycle()<<"\t"
    	   <<"RELEASE_LATENCY(NS): "<<overhead->getRELEASE_LATENCY()<<"\t"
	   <<std::endl;

  cout<<endl;
  taskSet->printParameters();
  
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
  return atoi(&res[0]);
}

long double SchedTest::getMsPerCycle() {

  long double sPerCycle;
  sPerCycle = 1.0/(__Hz_PER_MHz * __CPU_CLOCK_MHz);
  msPerCycle = sPerCycle * __MS_PER_S;
  return this->msPerCycle;
}

long double SchedTest::getNsPerCycle() {

  return getMsPerCycle() * __NS_PER_MS ;
}

   
