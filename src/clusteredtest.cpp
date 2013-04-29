#include "clusteredtest.hpp"
#include "globaltest.hpp"
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

  vector<unsigned>::iterator cacheTopIt;
  vector<Cluster*>::iterator clusterIt;
  Cluster *cluster;
  // cB is used to check if the new cluster is already added. i.e the same.
  bool cB;

  // first, for each cpu draw the corresponding cluster
  
  std::vector<vector<unsigned> >  cache_top = _cT.getCacheTop();
  for (unsigned i = 0; i< cache_top.size(); i++) {
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
  vector<unsigned>::iterator intit;
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

  map<pid_t,Task>::iterator it;
  for (it = taskSet->tasks.begin(); it != taskSet->tasks.end(); it++) {
    
    Task *task;
    task = taskSet->getTask(it->first);

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
  vector<unsigned>::iterator cpuIt;

  for (clusterIt = clustersVec.begin() ;
       clusterIt != clustersVec.end();
       clusterIt++) {

#ifdef PRINT_DEBUG_MSG    

    cout<<"Cluster has: "<<endl;
    // for (int i=0; i< (*clusterIt)->taskSet.getNbrTasks(); i++) {

      map<pid_t,Task>::iterator it;
      for (it = (*clusterIt)->taskSet.tasks.begin(); 
	   it != (*clusterIt)->taskSet.tasks.end(); 
	   it++) {

      cout<<"task id:"<<it->first<<";"<<"task cpu:"<<it->second.getCpu()<<";"<<endl;;
    }

#endif
      
    // Make a seperate schedulability test for each cluster
      

    if ((*clusterIt)->nbrCpus>1) {
      // apply a global schedulability test on this cluster

      GlobalTest globalTest;  
      globalTest.setTaskSet(&(*clusterIt)->taskSet);
      globalTest.setOverhead(overhead);
      globalTest.setMHzCpuClock(this->getMHzCpuClock());
      globalTest.setNbrCpus((*clusterIt)->nbrCpus);

      if (!globalTest.makeSchedTest())
	schedTestResult = 0;

    } else {
      // apply a partitionned (with one cpu) schedulablity test on this cluster

      PartitionnedTest partitionnedTest;  
      partitionnedTest.setTaskSet(&(*clusterIt)->taskSet);
      partitionnedTest.setOverhead(overhead);
      partitionnedTest.setMHzCpuClock(this->getMHzCpuClock());
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

