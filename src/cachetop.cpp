#include "cachetop.hpp"
#include <cstdio>
#include <cstring>
#include <sstream>
#include <iostream>
#include <unistd.h>
// #define PRINT_DEBUG_MSG

static struct CacheLevelEntry cacheLevelTable[] = {
  {"L1",L1},
  {"L2",L2},
  {"L3",L3},
  {"ALL",ALL}
};

void CacheTop::updateCacheTop(SchedTestParam *_schedTestParam) {
  
  cache_top = _schedTestParam->getCacheTop();
}

vector<vector<unsigned> > CacheTop::getCacheTop() {
  
  return cache_top;
}

int CacheTop::getCacheLevel() {

  FILE *f;
  char cluster[10];
  int cache_level;

  f = fopen("/proc/litmus/plugins/C-EDF/cluster","r");
  fgets(cluster, sizeof cluster, f);
  fclose(f);
  
  for (unsigned i=0; i<((sizeof cacheLevelTable)/(sizeof cacheLevelTable[0])); i++)  {
    
    if (!strncmp(cacheLevelTable[i].litmusName,cluster, strlen(cacheLevelTable[i].litmusName)))
      cache_level = cacheLevelTable[i].level;
  }
  
  return cache_level;
}

void CacheTop::drawCacheTop() {
  unsigned nbr_cpu;
  nbr_cpu = sysconf(_SC_NPROCESSORS_ONLN);  

  int cache_level;
  cache_level = getCacheLevel();

  if (cache_level != ALL) {

    FILE *f;
    char buf[10];
    unsigned sharedCpuStartId, sharedCpuEndId;

    vector<vector<unsigned> > tmp_cache_top;
    tmp_cache_top.resize(nbr_cpu);
  
    stringstream ss;

    for (unsigned i=0; i<nbr_cpu; i++) {
      ss.str("");
      ss<<"/sys/devices/system/cpu/cpu"<<i<<"/cache/index"<<cache_level<<"/shared_cpu_list";

      f = fopen(ss.str().c_str(),"r");
      fgets(buf, sizeof buf, f);
      fclose(f);
      sscanf(buf,"%u-%u", &sharedCpuStartId, &sharedCpuEndId);
      for (unsigned j=sharedCpuStartId; j <= sharedCpuEndId; j++) {
	tmp_cache_top[i].push_back(j);
      }
    }

    // filter out duplicate clusters
  
    vector<unsigned>::iterator it1;
    vector<unsigned>::iterator it2;
    bool b1;

    // loop through all cpus
    for (unsigned i=0; i<nbr_cpu; i++) {
   
      for (it1=tmp_cache_top[i].begin(); it1!=tmp_cache_top[i].end(); it1++) {
	b1 = false;
  
	// loop through all clusters
	for (unsigned j=0; j<cache_top.size(); j++) {

	  for (it2=cache_top[j].begin(); it2!=cache_top[j].end(); it2++) {
	    if (*it1 == *it2) {
	      // there is one common cpu
	      b1 =true;
	      break;
	    }
	  }
	  if (b1)
	    break;
	}
	if (b1)
	  break;
      }
      if (!b1) 
	// there is at least one uncommon cpu
	cache_top.push_back(tmp_cache_top[i]);
    }
  } else {
    vector<unsigned> tmpC;
    for (unsigned i=0; i<nbr_cpu; i++) {
      tmpC.push_back(i);
    }
    cache_top.push_back(tmpC);
  }

#ifdef PRINT_DEBUG_MSG
  
  vector<unsigned>::iterator it;
  for (int i=0; i<nbr_cpu; i++) {
    printf("CPU %d is sharing cache level %d  with: ",i,cache_level);
    for (it = cache_top[i].begin();it != cache_top[i].end(); it++) {
      printf("%d, ",*it);
    }
    printf(".\n");
  }
#endif

}

