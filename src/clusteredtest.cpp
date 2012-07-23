#include "clusteredtest.hpp"
#include "densitytest.hpp"
#include "partitionnedtest.hpp"
#include <vector>
#define PRINT_DEBUG_MSG


using namespace std;

ClusteredTest::ClusteredTest() {
}

ClusteredTest::~ClusteredTest() {
}

void ClusteredTest::setCacheTop(CacheTop *_cacheTop) {
  cacheTop = _cacheTop;
}

void ClusteredTest::drawClusters(CacheTop & _cT) {

  vector<int>::iterator cacheTopIt;
  vector<Cluster*>::iterator clusterIt;
  Cluster *cluster;
  // cB is used to check if the new cluster is already added. i.e the same.
  bool cB;

  // first, for each cpu draw the corresponding cluster
  
  std::vector<vector<int> >  cache_top = _cT.getCacheTop();
  for (int i = 0; i< cache_top.size(); i++) {
    cluster = new Cluster();    
    cB = false;

    for (cacheTopIt = cache_top[i].begin(); 
    	 cacheTopIt != cache_top[i].end(); 
    	 cacheTopIt++){
      cluster->addCpu(*cacheTopIt);
    }


    if (!cB) {
      clustersVec.push_back(cluster);
    }
  }

#ifdef PRINT_DEBUG_MSG

  // print clusters
  int i =0;
  vector<int>::iterator intit;
  cout<<"printing available clusters: "<<endl;
  for (clusterIt = clustersVec.begin(); 
       clusterIt != clustersVec.end(); 
       clusterIt++) {
    i++;
    cout<<"cluster"<<i<<"comprises the following cpus: "<<endl;
    for (intit = (*clusterIt)->cpus.begin();
	 intit != (*clusterIt)->cpus.end();
	 intit++)
    cout<<*intit<<";";
    
    cout<<endl;
  }

#endif

  // add tasks to their corresponding cluster

  for (int i=0; i< this->taskSet->getNbrTasks(); i++) {
    
    pid_t taskId;
    Task *task;
    taskId = taskSet->getTaskId(i);
    task = taskSet->getTask(taskId);

    for (clusterIt = clustersVec.begin(); 
	 clusterIt != clustersVec.end(); 
	 clusterIt++) {

      if ((*clusterIt)->inCluster(task)) {
	(*clusterIt)->taskSet.addTask(task);
      }     
    }
  }  
}

int ClusteredTest::makeSchedTest() {
  
  CacheTop cT;

  // we assume the task set is schedulable, unless otherwise correct
  int schedTestResult = 1;

  drawClusters(*cacheTop);

  vector<Cluster*>::iterator clusterIt;
  vector<int>::iterator cpuIt;

  for (clusterIt = clustersVec.begin() ;
       clusterIt != clustersVec.end();
       clusterIt++) {

#ifdef PRINT_DEBUG_MSG    

    cout<<"Cluster has: "<<endl;
    for (int i=0; i< (*clusterIt)->taskSet.getNbrTasks(); i++) {
      pid_t taskId;
      Task *task;
      taskId = (*clusterIt)->taskSet.getTaskId(i);
      task = (*clusterIt)->taskSet.getTask(taskId);
      cout<<"task id:"<<taskId<<";"<<"task cpu:"<<task->getCpu()<<";"<<endl;;
    }

#endif
      
    // Make a seperate schedulability test for each cluster
      

    if ((*clusterIt)->nbrCpus>1) {
      // apply a global schedulability test on this cluster

      DensityTest densityTest;  
      densityTest.setTaskSet(&(*clusterIt)->taskSet);
      densityTest.setOverhead(overhead);
      densityTest.setNbrCpus((*clusterIt)->nbrCpus);

      if (!densityTest.makeSchedTest())
	schedTestResult = 0;

    } else {
      // apply a partitionned (with one cpu) schedulablity test on this cluster

      PartitionnedTest partitionnedTest;  
      partitionnedTest.setTaskSet(&(*clusterIt)->taskSet);
      partitionnedTest.setOverhead(overhead);
      // partitionnedTest.setNbrCpus((*clusterIt)->nbrCpus);
      partitionnedTest.setCpus((*clusterIt)->cpus);

      if (!partitionnedTest.makeSchedTest())
	schedTestResult = 0;
    }
  }

  (schedTestResult ==1) ? 
    cout<<"Task set schedulable under C-EDF":
    cout<<"Task set unschedulable under C-EDF"; 
  

  return schedTestResult;
}  

