#include "densitytest.hpp"

DensityTest::DensityTest()
 :SchedTest() {
  this->schedTestName = DENSITY_TEST;
}

DensityTest::~DensityTest() {
}

void DensityTest::drawTaskSetSafeApprox() {

  // Preemption-centric interrupt accounting for global EDF 
  // without dedicated interrupt handling.

  long double sum_u_irq;  
  long double u0_tck;
  long double e0_tck;
  long double tmp_sum;
  long double c_pre;

  // First we computer the sum u_{j}^{irq} as it we need it later.
  sum_u_irq = 0.0;
  for (int i=0; i< taskSet->getNbrTasks(); i++) {

    pid_t taskId;
    taskId = taskSet->getTaskId(i);

    sum_u_irq += (long double) ((long double) ((long double)((long double)overhead->getTICK()
							     * (long double) getNsPerCycle() )
					       +(long double)overhead->getCPMD()) 
				/ (long double)taskSet->getTaskPeriod(taskId));      
  }

  // We computer u_{0}^{tck}, e_{0}^{tck}.
  u0_tck = 0.0;
  u0_tck = (long double) ((long double) ((long double) ((long double)overhead->getTICK()*
							(long double)getNsPerCycle())
					 +(long double)overhead->getCPMD()) 
			  / (long double)getNsTimerPeriod());      
  e0_tck = 0.0;
  e0_tck = (long double) ((long double) ( (long double)overhead->getTICK()
					  * (long double)getNsPerCycle() )
			  +(long double)overhead->getCPMD() );

  // We compute c^{pre}, the cost of one preemption.
  // This requires first computing the following sum:
  tmp_sum = 0;
  for (int i=0; i< taskSet->getNbrTasks(); i++) {

    pid_t taskId;
    taskId = taskSet->getTaskId(i);

    tmp_sum += (long double)((long double) ( (long double) overhead->getRELEASE_LATENCY()
					     * (long double) ((long double)((long double)((long double)overhead->getTICK()
											  *(long double)getNsPerCycle() )
									      +(long double)overhead->getCPMD()) 
								/ (long double)taskSet->getTaskPeriod(taskId)))
			     + (long double) ((long double) ((long double)overhead->getRELEASE()
							     *(long double)getNsPerCycle())
					      +(long double)overhead->getCPMD()));      
  }
  // c^{pre}:
  c_pre = 0;
  c_pre = (long double) ((long double) ((long double)e0_tck 
					+ ( (long double)((long double)(overhead->getRELEASE_LATENCY()) 
							  * (long double)u0_tck))
					+ (long double)tmp_sum)
			 
			 / (long double) (1.0 - (long double)u0_tck - (long double)sum_u_irq));



  for (int i=0; i< this->taskSet->getNbrTasks(); i++) {
    
    pid_t taskId;
    long double execCost;
    long double period;    
    taskId = taskSet->getTaskId(i);
    
    // New execution cost.
    execCost = (long double)taskSet->getTaskExecCost(taskId);
    execCost = (long double)((long double)((long double)((long double)execCost
							 +(long double)(2.0
									*(long double)((long double)overhead->getSCHED()
										       +(long double)overhead->getSCHED2()
										       +(long double)overhead->getCXS())
									*(long double)getNsPerCycle())
							 +(long double)overhead->getCPMD())
					     
					     / (long double)((long double)1.0
							     -(long double)u0_tck
							     -(long double)sum_u_irq))
			     + (long double)(2.0
					     *(long double)c_pre)
			     + (long double)((long double)overhead->getSEND_RESCHED()
					     *(long double)getNsPerCycle()));

      
    taskSet->setTaskExecCost(taskId, execCost);

    // New period.
    period = (long double)taskSet->getTaskPeriod(taskId);
    period = (long double)(period 
			   -(long double)((long double)overhead->getSEND_RESCHED()
					  *(long double)getNsPerCycle()));

    taskSet->setTaskPeriod(taskId, period);

  }
}

int DensityTest::makeSchedTest() {
    
  int i;
  long double sum_density;
  long double max_density;
  long double cur_density;

  sum_density = 0.0;
  max_density = 0.0;
  cur_density = 0.0;

  drawTaskSetSafeApprox();

  for (i=0; i< this->taskSet->getNbrTasks(); i++) {
    
    pid_t taskId;
    taskId = taskSet->getTaskId(i);

    cur_density = 0.0;
    cur_density =
      (long double)( (long double) ( (long double)((this->taskSet->getTaskExecCost(taskId)))
				     +(long double)((this->taskSet->getTaskSelfSuspension(taskId)))
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
           <<"sum_overhead(NS): "<< overhead->getSumOverheads() <<"\t"
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

   
