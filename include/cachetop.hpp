#ifndef CACHE_TOP_HPP
#define CACHE_TOP_HPP

#include <vector>
#include <iostream>
#include "schedtestparam.hpp"

using namespace std;

enum CacheLevel {
  L1 = 1,
  L2,
  L3,
  ALL
};

struct CacheLevelEntry { 
  const char* litmusName;
  enum CacheLevel level;
};

class CacheTop {

private:
  vector<vector<unsigned> > cache_top;

public:

  void updateCacheTop(SchedTestParam*);

  vector<vector<unsigned> > getCacheTop();
  int getCacheLevel();
  void drawCacheTop();
};

#endif
