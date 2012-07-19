#include "cachetop.hpp"
#include <cstdio>
#include <cstring>
#include <sstream>
#include <iostream>
#include <unistd.h>

static struct CacheLevelEntry cacheLevelTable[] = {
  {"L1",L1},
  {"L2",L2},
  {"L3",L3},
  {"ALL",ALL}
};

vector<vector<int> > CacheTop::getCacheTop() {
  
  return cache_top;
}

int CacheTop::getCacheLevel() {

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

void CacheTop::drawCacheTop() {
  int cache_level;
  cache_level = getCacheLevel();

  FILE *f;
  char buf[10];
  unsigned sharedCpuStartId, sharedCpuEndId;

  int nbr_cpu;
  nbr_cpu = sysconf(_SC_NPROCESSORS_ONLN);  
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

#ifdef PRINT_DEBUG_MSG
  
  vector<int>::iterator it;
  for (int i=0; i<nbr_cpu; i++) {
    printf("CPU %d is sharing cache level %d  with: ",i,cache_level);
    for (it = cache_top[i].begin();it != cache_top[i].end(); it++) {
      printf("%d, ",*it);
    }
    printf(".\n");
  }
#endif

}

