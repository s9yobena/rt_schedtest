#ifndef CACHE_TOP_HPP
#define CACHE_TOP_HPP

#include <vector>
#include <iostream>

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
  vector<vector<int> > cache_top;
public:
  vector<vector<int> > getCacheTop();
  int getCacheLevel();
  void drawCacheTop();
};

#endif
