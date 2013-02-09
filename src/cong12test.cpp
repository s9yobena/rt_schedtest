#include "cong12test.hpp"
#include "task.hpp"
#include <cmath>
#include <algorithm>
#include <vector>

Cong12Test::Cong12Test() {
}

Cong12Test::~Cong12Test() {
}

// Helper functions. Definitions from Cong's paper, "Multiprocessor
// Schedulability Analysis for Self-Suspending Task Systems". 

// Demand Bound Function, used to compute the "maximum cumulative 
// execution requirement of jobs that both arrive in, and have
// deadlines within, any interval of length t".
long double Cong12Test::DBF(Task* _task, lt_t _t) {
  
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

long double Cong12Test::DELTA(Task* _task, lt_t _t) {
  
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
    
long double Cong12Test::W_nc(Task* _iTask, Task* _lTask,
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

long double Cong12Test::W_c(Task* _iTask, Task* _lTask,
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

int Cong12Test::makeSchedTest(long deltaSelfSusp, long deltaKsi) {

  long double ksi;
  long double lowerBoundKsi;
  long double upperBoundKsi;
  long double sumExecCost;
  long double sumUtilization;
  long double sumTardiness;
  unsigned nbrCmpTasks;		

  ksi            = 0.0;
  lowerBoundKsi  = 0.0;
  upperBoundKsi  = 0.0;
  sumExecCost    = 0.0;
  sumUtilization = 0.0;
  sumTardiness   = 0.0;
  nbrCmpTasks    = 0.0;

  drawTaskSetSafeApprox();

  for (int i=0; i< taskSet->getNbrTasks(); i++) {

    pid_t taskId;
    taskId = taskSet->getTaskId(i);
    
    // Compute the sum of the execution costs for all tasks.
    sumExecCost += (long double)(taskSet->getTaskExecCost(taskId));

    // Compute the utilization sum for all tasks.
    sumUtilization += (long double)(taskSet->getTaskUtilization(taskId));

    // Compute the tardiness sum for all tasks.
    sumTardiness += (long double)(taskSet->getTaskTardiness(taskId));

    // Compute the number of computational tasks (tasks that do not suspend).
    if (taskSet->getTaskSelfSuspension(taskId) == 0)
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

  // Make the test for each task.
  for (int l=0; l< taskSet->getNbrTasks(); l++) {
    
    pid_t lTaskId;
    Task *lTask;
    lTaskId = taskSet->getTaskId(l);
    lTask = taskSet->getTask(lTaskId);

    // Make the test for all possible values of per-job self-suspensions
    for (long perJobSelfSusp = 0; 
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

      // Make sure the lower bound is greater than the upper bound
      // if (lowerBoundKsi > upperBoundKsi) {

      // 	std::cout<<"lowerBoundKsi > upperBoundKsi "
      // 		 <<lowerBoundKsi
      // 		 <<" > "
      // 		 <<upperBoundKsi
      // 		 <<std::endl;
      // 	return 0;
      // }

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
	  
	for (int i=0; i< this->taskSet->getNbrTasks(); i++) {
		 
	  pid_t iTaskId;
	  Task *iTask;
	  iTaskId = taskSet->getTaskId(i);
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

	if (LHS <= RHS) {
	  
	  printf("\n*******************Cong Schedulability test result:***** TASK SET STILL SCHEDULABLE :-) ****************** \n");
	  std::cout<<"LHS <= RHS : "
		   <<LHS
		   <<" <= "
		   <<RHS
		   <<std::endl;
	  return 1;
	} else {

	  printf("\n*******************Cong Schedulability test result:***** TASK SET NO LONGER SCHEDULABLE :-( ************** \n");
	  std::cout<<"LHS > RHS : "
		   <<LHS
		   <<" > "
		   <<RHS
		   <<std::endl;
	  return 0;
	}
	ksi += deltaKsi;
	} while (ksi < upperBoundKsi);// end for each ksi
    } // end for each self suspension
  } // end for each lTask




}   
