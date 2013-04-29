#include "globaltest.hpp"
#include "task.hpp"
#include <cmath>
#include <algorithm>
#include <vector>
#include <omp.h>
#define PARALLEL

// #define PRINT_SCHED_OUTPUT

GlobalTest::GlobalTest() {
  dedicatedIH = true;
}

GlobalTest::~GlobalTest() {
}

void GlobalTest::drawTaskSetSafeApprox() {

  // Preemption-centric interrupt accounting for global EDF 
  // without dedicated interrupt handling.

  long double sum_u_irq;  
  long double u0_tck;
  long double e0_tck;
  long double tmp_sum;
  long double c_pre;

  // First we computer the sum u_{j}^{irq} as it we need it later.
  sum_u_irq = 0.0;
  map<pid_t,Task>::iterator it;
  for (it = taskSet->tasks.begin(); it != taskSet->tasks.end(); it++) {

    sum_u_irq += (long double) ((long double) ((long double)((long double)overhead->getTICK()
							     * (long double) getNsPerCycle() )
					       +(long double)overhead->getCPMD()) 
				/ (long double)it->second.getPeriod());
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
  for (it = taskSet->tasks.begin(); it != taskSet->tasks.end(); it++) {

    tmp_sum += (long double)((long double) ( (long double) overhead->getRELEASE_LATENCY()
					     * (long double) ((long double)((long double)((long double)overhead->getTICK()
											  *(long double)getNsPerCycle() )
									      +(long double)overhead->getCPMD()) 
							      / (long double)it->second.getPeriod()))
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


  for (it = taskSet->tasks.begin(); it != taskSet->tasks.end(); it++) {

    long double execCost;
    long double period;    
      
    // New execution cost.
    execCost = (long double)it->second.getExecCost();
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

      
    taskSet->setTaskExecCost(it->first, execCost);

    // New period.
    period = (long double)it->second.getPeriod();
    period = (long double)(period 
			   -(long double)((long double)overhead->getSEND_RESCHED()
					  *(long double)getNsPerCycle()));

    taskSet->setTaskPeriod(it->first, period);
  }
}

void GlobalTest::drawTaskSetSafeApprox_DIH() {

  // Preemption-centric interrupt accounting for global EDF 
  // with Dedicated Interrupt Handling.

  long double u0_tck;
  long double e0_tck;
  long double c_pre;

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
  c_pre = 0;
  c_pre = (long double) ((long double) ((long double)e0_tck 
					+ ( (long double)((long double)(overhead->getRELEASE_LATENCY()) 
							  * (long double)u0_tck)))
			 
			 / (long double) (1.0 - (long double)u0_tck));



  map<pid_t,Task>::iterator it;
  for (it = taskSet->tasks.begin(); it != taskSet->tasks.end(); it++) {

    long double execCost;
    long double period;    
    
    // New execution cost.
    execCost = (long double)it->second.getExecCost();
    execCost = (long double)((long double)((long double)((long double)execCost
							 +(long double)(2.0
									*(long double)((long double)overhead->getSCHED()
										       +(long double)overhead->getSCHED2()
										       +(long double)overhead->getCXS())
									*(long double)getNsPerCycle())
							 +(long double)overhead->getCPMD())
					     
					     / (long double)((long double)1.0
							     -(long double)u0_tck))
			     + (long double)(2.0
					     *(long double)c_pre)
			     + (long double)((long double)overhead->getSEND_RESCHED()
					     *(long double)getNsPerCycle())
			     + (long double)((long double)overhead->getRELEASE()
					     *(long double)getNsPerCycle()));

      
    taskSet->setTaskExecCost(it->first, execCost);

    // New period.
    period = (long double)it->second.getPeriod();
    period = (long double)(period 
			   -(long double)((long double)overhead->getSEND_RESCHED()
					  *(long double)getNsPerCycle()));

    taskSet->setTaskPeriod(it->first, period);
  }
}

int GlobalTest::makeDensityTest() {
    
  long double sum_density;
  long double max_density;
  long double cur_density;

  sum_density = 0.0;
  max_density = 0.0;
  cur_density = 0.0;

  map<pid_t,Task>::iterator it;
  for (it = taskSet->tasks.begin(); it != taskSet->tasks.end(); it++) {

    cur_density = 0.0;
    cur_density =
      (long double)( (long double) ( (long double)((it->second.getExecCost()))
				     +(long double)((it->second.getSelfSuspension()))
				     ) 
		     /(long double)((it->second.getPeriod())));
    
    if (cur_density > max_density) {
      max_density = cur_density;
    }
    sum_density += cur_density;    
  }

#ifdef PRINT_SCHED_OUTPUT

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

  std::cout<<"CPU CLOCK MHz: "<<getMHzCpuClock()<<"\t"
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

#endif
  
  if (sum_density <= (long double)nbr_cpu-((long double)nbr_cpu-1)*max_density) {
#ifdef PRINT_SCHED_OUTPUT
    printf("\n*******************Schedulability test result:***** TASK SET STILL SCHEDULABLE :-) ****************** \n");
#endif
    return 1;
  } else {
#ifdef PRINT_SCHED_OUTPUT
    printf("\n*******************Schedulability test result:***** TASK SET NO LONGER SCHEDULABLE :-( ************** \n");
#endif
    return 0;
  }  

}

// Helper functions. Definitions from Cong's paper, "Multiprocessor
// Schedulability Analysis for Self-Suspending Task Systems". 

// Demand Bound Function, used to compute the "maximum cumulative 
// execution requirement of jobs that both arrive in, and have
// deadlines within, any interval of length t".
long double GlobalTest::DBF(Task* _task, lt_t _t) {
  
  long double tmp;
  
  tmp = (long double) ((long double) ( floor((long double)( (long double)( (long double)(_t)
									   - (long double)(_task->getDeadline()))
							    / (long double)(_task->getPeriod())))
				       + 1)
		       *(long double)(_task->getExecCost()));
  
  return max(tmp, (long double)0);
}

// Delta "is defined for computing carry-in workloads", i.e. like 
// DBF but takes also carry-in jobs into account.

long double GlobalTest::DELTA(Task* _task, lt_t _t) {
  
  long double tmp0;
  long double tmp1;
  long double tmp2;

  tmp0 = (long double) ((long double) ( ceil((long double)( (long double)(_t)
							    / (long double)(_task->getPeriod())))
					- 1)
			*(long double)(_task->getExecCost()));

  tmp1 = (long double) ((long double) ( (long double) (_t) 
					- (long double) ( ceil((long double)( (long double)(_t)
									      / (long double)(_task->getPeriod()) ))
							  * (long double)(_task->getPeriod()))
					+ (long double)(_task->getPeriod()) ));
  
  tmp2 = min( (long double)_task->getExecCost(), tmp1);

    

  return tmp0 + tmp2;
}
    
long double GlobalTest::W_nc(Task* _iTask, Task* _lTask,
			     long double _lKsi, long double _lPerJobSelfSusp ) {
  
  long double tmp0;

  tmp0 = DBF(_iTask, (_lKsi - _lTask->getTardiness()) );

  if (_iTask->getId() != _lTask->getId() ) {
    
    return min( tmp0,
		(long double) (_lKsi
			       - (long double)(_lTask->getExecCost())
			       - _lPerJobSelfSusp
			       +1));
  } else {
    
    return min( tmp0 - (long double)(_lTask->getExecCost()), 
    		max ((long double) (_lKsi
    				    - (long double)(_lTask->getTardiness())
    				    - (long double)(_lTask->getDeadline())) // change to deadline when switching to explicit deadlines
    		     ,_lKsi - (long double) (_lTask->getPeriod())));
  }
}

long double GlobalTest::W_c(Task* _iTask, Task* _lTask,
			    long double _lKsi, long double _lPerJobSelfSusp ) {
  

  long double tmp0;

  if (_iTask->getId() != _lTask->getId() ) {

    tmp0 = DELTA(_iTask, (_lKsi - _lTask->getTardiness() + _iTask->getTardiness()) );
    
    return min( tmp0,
		(long double) (_lKsi
			       - (long double)(_lTask->getExecCost())
			       - _lPerJobSelfSusp
			       +1));
  } else {
    
    tmp0 = DELTA(_lTask, _lKsi);

    return min( tmp0 - (long double)(_lTask->getExecCost()), 
		max ((long double) (_lKsi
				    - (long double)(_lTask->getTardiness())
				    - (long double)(_lTask->getDeadline())) // change to deadline when switching to explicit deadlines
		     , _lKsi - (long double) (_lTask->getPeriod())));
  
  }
}

bool compF (long double i, long double j) { return (i>j); }

int GlobalTest::makeCong12Test(long deltaSelfSusp, long deltaKsi) {


  
  int schedTestResult;
  long double sumExecCost;
  long double sumUtilization;
  long double sumTardiness;
  unsigned nbrCmpTasks;		

  schedTestResult = 1;
  sumExecCost    = 0.0;
  sumUtilization = 0.0;
  sumTardiness   = 0.0;
  nbrCmpTasks    = 0.0;

  map<pid_t,Task>::iterator it;
  for (it = taskSet->tasks.begin(); it != taskSet->tasks.end(); it++) {

    // Compute the sum of the execution costs for all tasks.
    sumExecCost += (long double)(it->second.getExecCost());

    // Compute the utilization sum for all tasks.
    sumUtilization += (long double)(it->second.getUtilization());

    // Compute the tardiness sum for all tasks.
    sumTardiness += (long double)(it->second.getTardiness());

    // Compute the number of computational tasks (tasks that do not suspend).
    if (it->second.getSelfSuspension() == 0)
      nbrCmpTasks++ ;
  }

  // If the utilization sum is greater than the number of processors, report non schedulability
  if (sumUtilization > nbr_cpu) {
    printf("\n*******************Cong'Schedulability test result:***** TASK SET NO LONGER SCHEDULABLE :-( ************** \n");
    std::cout<<"sumUtilization > nbr_cpu : "
  	     <<sumUtilization
  	     <<" > "
  	     <<nbr_cpu
  	     <<std::endl;

    return 0;
  }

  it = taskSet->tasks.begin();  

#ifdef PARALLEL  
#pragma omp parallel shared(schedTestResult, sumExecCost, sumUtilization, sumTardiness, nbrCmpTasks)
  {
#endif

#ifdef PARALLEL
#pragma omp for schedule(dynamic) nowait
#endif
    // Make the test for each task.
    for (int i = 0; i< taskSet->getNbrTasks(); i++) {

      long double ksi;
      long double lowerBoundKsi;
      long double upperBoundKsi;
      int breakKsiLoop;
      int  breakSelfSuspLoop;

      ksi               = 0.0;
      lowerBoundKsi     = 0.0;
      upperBoundKsi     = 0.0;
      breakKsiLoop      = 0;
      breakSelfSuspLoop = 0;

#ifdef PRINT_SCHED_OUTPUT
      cout<<omp_get_num_threads()<<" executing this region,"<<endl;
#endif
      pid_t lTaskId;
      Task *lTask;
      lTaskId = it->first;
      lTask = taskSet->getTask(lTaskId);

      // Make the test for all possible values of per-job self-suspensions
      for (lt_t perJobSelfSusp = 0; 
	   perJobSelfSusp < lTask->getPerJobMaxSelfSusp(); 
	   perJobSelfSusp += deltaSelfSusp) {


	// Under this litmus version, implicit deadlines are assumed. 
	// FIXME: add tardiness threshold when adding SRT support
	lowerBoundKsi = min ((long double)((long double)(lTask->getDeadline())
					   +  (long double)(lTask->getTardiness()))
			     ,(long double)(lTask->getPeriod()));

	upperBoundKsi =(long double)( (long double) ((long double)( (long double)(nbr_cpu)
								    *(long double)( (long double) (lTask->getExecCost()) 
										    + (long double)(perJobSelfSusp)))
						     -(long double)( (long double)(lTask->getTardiness()) 
								     *(long double)(sumUtilization))
						     +(long double)((long double)(lTask->getTardiness()) 
								    *(long double)(lTask->getUtilization())) 
						     +(long double)(sumExecCost))
				      /(long double) ( (long double)(nbr_cpu)
						       - (long double) (sumUtilization)));

	// Make the test for all possible values of KSI
	ksi = lowerBoundKsi;
	do {

	  // make the schedulability test on the task lTaksId; This requires
	  // taking all the tasks into account.
	  long double tmpSum0;
	  long double tmpSum1;
	  long double tmpSum2;
	  vector<long double> tmpVec; // used to compute the max n elements, see below.
	  vector<long double>::iterator tmpIt;
	  unsigned tmpV;

	  
	  tmpSum0 = 0.0;
	  tmpSum1 = 0.0;
	  tmpSum2 = 0.0;
	  tmpVec.clear();
	  tmpV    = 0;
	  
	  for (it = taskSet->tasks.begin(); it != taskSet->tasks.end(); it++) {
	    
	    pid_t iTaskId;
	    Task *iTask;
	    iTaskId = it->first;
	    iTask = taskSet->getTask(iTaskId);
	    
	    tmpSum0 += max(W_nc(iTask, lTask, ksi, perJobSelfSusp)
			   , W_c(iTask, lTask, ksi, perJobSelfSusp));

	    // The schedulability test distinguished between self suspending
	    // tasks and computational tasks (which do not suspend).
	    if (iTask->getSelfSuspension() == 0) {
	      tmpSum1 += W_nc(iTask, lTask, ksi, perJobSelfSusp);

	      // Next, we compute the "min(m − 1, ne ) greatest val-
	      // ues of max(0, Wc (Ti ) − Wnc (Ti )) for any computational
	      // task Ti", where ne is the number of computational tasks, 
	      // this is done in two steps; S.1 put all the value in a vector;
	      // S.2 get the max min(m − 1, ne ) elements (STL provides a method,
	      //  nth_element(), that does so in linear time "on average")
	      	      
	      // S.1 

	      tmpV = min((unsigned)nbr_cpu-1,nbrCmpTasks);
	      tmpVec.push_back( max((long double)0, (W_c(iTask,lTask, ksi, perJobSelfSusp) - W_nc(iTask,lTask, ksi, perJobSelfSusp))));
	      nth_element (tmpVec.begin()
			   ,tmpVec.begin()+tmpV
			   ,tmpVec.end()
			   ,compF);
	    }
	    // S.2
	    for (tmpIt=tmpVec.begin(); tmpIt!=tmpVec.begin()+tmpV; ++tmpIt)
	      tmpSum2 += *tmpIt;
	  
	  } // end for each iTask
	
	  // And there we go, the schedulability test.
	  double long LHS;
	  double long RHS;

	  LHS = tmpSum0 + tmpSum1 + tmpSum2;

	  RHS = (double long)((unsigned long)(nbr_cpu)
			      *(unsigned long)(ksi
					       -(long double)(lTask->getExecCost())
					       -perJobSelfSusp));

	  if (LHS > RHS) {

            #pragma omp critical
	    {
	      schedTestResult = 0;
	      printf("\n*******************Cong Schedulability test:\n");
	      std::cout<<"Task: "<<lTask->getId()<<std::endl;
	      std::cout<<"Failed to pass cong's test "<<std::endl;
	      std::cout<<"LHS > RHS : "
		       <<LHS
		       <<" > "
		       <<RHS<<endl;
	    }
            #pragma omp flush(schedTestResult)
	  }

          #pragma omp critical
	  {
	    if (schedTestResult == 0) {
	      breakKsiLoop = 1;
	      breakSelfSuspLoop = 1;
	    }
	  }


	  ksi += deltaKsi;
	  if (breakKsiLoop)
	    break;
	  
	} while (ksi < upperBoundKsi);// end for each ksi

	if (breakSelfSuspLoop) 
	  break;
      } // end for each self suspension
      
      it++;
    } // end for each lTask

#ifdef PARALLEL
  } // end  #pragma omp parallel
#endif

  if (schedTestResult) {
#ifdef PRINT_SCHED_OUTPUT
    printf("\n*******************Cong Schedulability test result:***** ALL TASKS SCHEDULABLE :-) ****************** \n");
#endif    
    return 1;
  } else {
#ifdef PRINT_SCHED_OUTPUT
    printf("\n*******************Cong Schedulability test result:***** TASK SET NON SCHEDULABLE :-( ****************** \n");
#endif
    return 0;

  }   
}

int GlobalTest::makeSchedTest() {

  if (!dedicatedIH) {

    drawTaskSetSafeApprox();
  } else {
    
    if (nbr_cpu>1)
      nbr_cpu--;

    drawTaskSetSafeApprox_DIH();
  }

  if (makeDensityTest())
    return 1;
  else
    return makeCong12Test(20,20);
}









