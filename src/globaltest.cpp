#include "globaltest.hpp"

GlobalTest::GlobalTest() {
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

