#include "clusteredtest.hpp"
#include <vector>

using namespace std;

#define ALL -1

static struct CacheLevelEntry cacheLevelTable[] = {
  {"L1",1},
  {"L2",2},
  {"L3",3},
  {"ALL",ALL}
};


ClusteredTest::ClusteredTest() {
}

ClusteredTest::~ClusteredTest() {
}

void ClusteredTest::drawClusters() {

  vector<int>::iterator cacheTopIt;
  vector<Cluster*>::iterator clusterIt;
  Cluster *cluster;
  // cB is used to check if the new cluster is already added. i.e the same.
  bool cB;

  // first, for each cpu draw the corresponding cluster
  for (int i = 0; i< nbr_cpu; i++) {
    cluster = new Cluster();    
    cB = false;
    for (cacheTopIt = cache_top[i].begin(); 
	 cacheTopIt != cache_top[i].end(); 
	 cacheTopIt++){
      cluster->addCpu(*cacheTopIt);
    }

    // then check if the cluster is already stored
    for (clusterIt = clustersVec.begin(); 
	 clusterIt != clustersVec.end(); 
	 clusterIt++) {
      
      if ((*clusterIt)->isIdentical(cluster)) {
	cB = true;
	break;
      }     
    }

    if (!cB) {
      clustersVec.push_back(cluster);
    }
  }

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

  // add tasks to their corresponding cluster

  for (int i=0; i< this->taskSet->getNbrTasks(); i++) {
    
    cout<<"we have"<<this->taskSet->getNbrTasks()<<" tasks."<<endl;
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

int ClusteredTest::getCacheLevel() {

  FILE *f;
  char cluster[10];
  int cache_level;

  f = fopen("/proc/litmus/plugins/C-EDF/cluster","r");
  fgets(cluster, sizeof cluster, f);
  fclose(f);
  
  for (int i=0; i<((sizeof cacheLevelTable)/(sizeof cacheLevelTable[0])); i++)  {
    
    if (!strncmp(cacheLevelTable[i].litmusName,cluster, strlen(cacheLevelTable[i].litmusName)))
      cache_level = cacheLevelTable[i].level;
  }
  
  return cache_level;
}

void ClusteredTest::drawCacheTop() {
  int cache_level;
  cache_level = getCacheLevel();

  FILE *f;
  char buf[10];
  unsigned sharedCpuStartId, sharedCpuEndId;
  
  // cout<<"the vector has "<<cache_top[0].size()<<endl;
  cache_top.resize(nbr_cpu);
  stringstream ss;

  for (int i=0; i<nbr_cpu; i++) {
    ss.str("");
    ss<<"/sys/devices/system/cpu/cpu"<<i<<"/cache/index"<<cache_level<<"/shared_cpu_list";

    f = fopen(ss.str().c_str(),"r");
    fgets(buf, sizeof buf, f);
    fclose(f);
    sscanf(buf,"%u-%u", &sharedCpuStartId, &sharedCpuEndId);
    for (int j=sharedCpuStartId; j <= sharedCpuEndId; j++) {
      cache_top[i].push_back(j);
    }
  }

  vector<int>::iterator it;

  for (int i=0; i<nbr_cpu; i++) {
    printf("CPU %d is sharing cache level %d  with: ",i,cache_level);
    for (it = cache_top[i].begin();it != cache_top[i].end(); it++) {
      printf("%d, ",*it);
    }
    printf(".\n");
  }

}

int ClusteredTest::makeSchedTest() {
  
  if (getCacheLevel() != ALL) {
    drawCacheTop();
    drawClusters();

    vector<Cluster*>::iterator clusterIt;
    vector<int>::iterator cpuIt;

    for (clusterIt = clustersVec.begin() ;
	 clusterIt != clustersVec.end();
	 clusterIt++) {
    
      cout<<"Cluster has: "<<endl;

      for (int i=0; i< (*clusterIt)->taskSet.getNbrTasks(); i++) {
    
	pid_t taskId;
	Task *task;
	taskId = (*clusterIt)->taskSet.getTaskId(i);
	task = (*clusterIt)->taskSet.getTask(taskId);

	cout<<"task id:"<<taskId<<";"<<"task cpu:"<<task->getCpu()<<";"<<endl;;
      }
    }
  } else {
    
    cout<<"making global schedulability testing"<<endl;
  }
}  

