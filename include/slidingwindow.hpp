#ifndef SLIDING_WINDOW_HPP
#define SLIDING_WINDOW_HPP

#include <iostream>
#include <list>
#include <vector>

using namespace std;

template <class T>
class SlidingWindow {
    
private:
  list<T> slidingWindow;
  unsigned maxSize;
  int currSize;
  float percentile;
    
    
public:

  SlidingWindow() {
    maxSize = 100;
    percentile = 0.6;
  }

  void setMaxSize(int s) {
    maxSize = s;
  }

  void setPrcntl(int _prcntl) {
    percentile = _prcntl;
  }
    
  void update (T x) {
        
    if (slidingWindow.size() < maxSize) {

      slidingWindow.push_back(x);
      currSize++;
    } else {

      slidingWindow.pop_front();
      slidingWindow.push_back(x);
    }
  }

  T computeAvrg() {
    list<T> tmpSW(slidingWindow);
    tmpSW.sort();
    typename list<T>::iterator it;
    vector<T> tmpVec;    
    typename vector<T>::iterator vecIt;    

    for (it = tmpSW.begin();
  	 it != tmpSW.end();
  	 it++) {
      
      tmpVec.push_back(*it);
    }

    int idxPrcntl;
    
    T sum = 0;
    for (int i = 0; i<= idxPrcntl; i++) {
      sum += tmpVec[i];
    }
   
    return ((sum/(idxPrcntl+1)) +1);
  }
};

#endif
