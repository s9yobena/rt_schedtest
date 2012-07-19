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
  vector<int> cpus;
  TaskSet taskSet;
  
  Cluster() {
    nbrCpus = 0;
  }

  void addCpu(int i) {
    cpus.push_back(i);
    nbrCpus++;
  }

  bool isIdentical(Cluster *_cluster) {
    vector<int>::iterator it1;
    vector<int>::iterator it2;
    bool b1;
    for (it1=cpus.begin(); it1!=cpus.end(); it1++) {
      b1 = false;
      for (it2=_cluster->cpus.begin(); it2!=_cluster->cpus.end(); it2++) {
	if (*it1 == *it2) {
	  b1 =true;
	}
      }
      if (!b1) {
	// there is at least one uncommon cpu
	return false;
      }
    }
    return true;
  }

  bool inCluster(Task *task) {
    vector<int>::iterator it;
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

public :
  ClusteredTest();
  ~ClusteredTest();
  void drawClusters(CacheTop &);
  
  int makeSchedTest();
};


#endif
