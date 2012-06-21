#include "densitytest.hpp"

DensityTest::DensityTest()
 :SchedTest() {
  this->schedTestName = DENSITY_TEST;
}

DensityTest::~DensityTest() {
}

int DensityTest::makeSchedTest() {
    
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

  for (i=0; i< this->taskSet->getNbrTasks(); i++) {
    
    pid_t taskId;
    taskId = taskSet->getTaskId(i);

    cur_density = 0.0;
    cur_density =
      (long double)( (long double) ( (long double)((this->taskSet->getTaskExecCost(taskId)))
				     +(long double)(inflation) 
				     ) 
		     /(long double)((this->taskSet->getTaskPeriod(taskId))));
    
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

   
