#include "partitionnedtest.hpp"
#include <map>

using namespace std;

PartitionnedTest::PartitionnedTest() {
}

PartitionnedTest::~PartitionnedTest() {
}

void PartitionnedTest::setCpus(vector<int> _cpus) {
  cpus = _cpus;
}

void PartitionnedTest::drawTaskSetSafeApprox() {

  // Each cpus handles its interrupts. i.e. we 
  // don't use dedicated interrupt handling.


  for (int i=0; i< this->taskSet->getNbrTasks(); i++) {
    
    pid_t taskId;
    long double execCost;
    long double selfSusp;    
    taskId = taskSet->getTaskId(i);
    
    // New execution cost.
    execCost = (long double)taskSet->getTaskExecCost(taskId);
    execCost = (long double)((long double)execCost
			     +(long double)(2.0
					    *(long double)((long double)overhead->getSCHED()
							   +(long double)overhead->getSCHED2()
							   +(long double)overhead->getCXS())
					    *(long double)getNsPerCycle())
			     +(long double)overhead->getCPMD());

      
    taskSet->setTaskExecCost(taskId, execCost);

    // New self suspension.
    selfSusp = (long double)taskSet->getTaskSelfSuspension(taskId);
    selfSusp = (long double)((long double)selfSusp
				 +(long double)((long double)((long double)overhead->getRELEASE()
							      +(long double)overhead->getSEND_RESCHED())
						*(long double)getNsPerCycle())
			     +(long double)overhead->getRELEASE_LATENCY());
				 
    taskSet->setTaskSelfSuspension(taskId, selfSusp);
  }
}



int PartitionnedTest::makeSchedTest() {

  // Under partitionned scheduling under m processor, 
  // we first divide the tasks into m partitions, then
  // apply exact, uniprocessor EDF schedulability test.

  map<int,double> partitionsUtilization;
  map<int,double>::iterator puItr;
  vector<int>::iterator cpuItr;
  for (cpuItr = cpus.begin(); cpuItr != cpus.end(); cpuItr++) {
    partitionsUtilization.insert( pair<int,double>(*cpuItr,0.0) );
  }

  drawTaskSetSafeApprox();

  for (int i=0; i< this->taskSet->getNbrTasks(); i++) {
    
    pid_t taskId;
    taskId = taskSet->getTaskId(i);


    partitionsUtilization[taskSet->getTaskCpu(taskId)] +=
      (long double)( (long double) ( (long double)((this->taskSet->getTaskExecCost(taskId)))
				     +(long double)((this->taskSet->getTaskSelfSuspension(taskId)))
				     ) 
		     /(long double)((this->taskSet->getTaskPeriod(taskId))));
  }
  printf("\n");
  printf("******************************Testing schedulability********************************************\n");

  for (puItr = partitionsUtilization.begin(); 
       puItr != partitionsUtilization.end(); puItr++) {

    std::cout<<"Partition's cpu : "<<puItr->first<<"\t"
	     <<"sum_utilization: "<<puItr->second <<"\t"
	     <<std::endl;
  }

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
  
  for (puItr = partitionsUtilization.begin(); 
       puItr != partitionsUtilization.end(); puItr++) {

    if (puItr->second > 1) {
      printf("\n*******************Schedulability test result:***** TASK SET NO LONGER SCHEDULABLE :-( ************** \n");    return 0;
    }
  }
  printf("\n*******************Schedulability test result:***** TASK SET STILL SCHEDULABLE :-) ****************** \n");
  return 1;
}  

