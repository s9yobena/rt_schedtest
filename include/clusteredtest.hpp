#ifndef CLUSTERED_TEST_H
#define CLUSTERED_TEST_H

#include "schedtest.hpp"
#include <vector>
#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include "taskset.hpp"
#include "cachetop.hpp"


class Cluster {

public:
  unsigned nbrCpus;
  vector<unsigned> cpus;
  TaskSet taskSet;
  
  Cluster() {
    nbrCpus = 0;
  }

  void addCpu(int i) {
    cpus.push_back(i);
    nbrCpus++;
  }

  bool inCluster(Task *task) {
    vector<unsigned>::iterator it;
    for (it=cpus.begin(); it!=cpus.end(); it++) {
      if (task->getCpu()==*it) {
	return true;
      }
    }
    return false;
  }
};

class ClusteredTest : public SchedTest{

protected:
  void drawTaskSetSafeApprox();
  vector<Cluster*> clustersVec;
  CacheTop *cacheTop;

public :
  ClusteredTest();
  ~ClusteredTest();

  void setCacheTop(CacheTop*);
  void drawClusters(CacheTop &);

  int makeSchedTest();
};


#endif
